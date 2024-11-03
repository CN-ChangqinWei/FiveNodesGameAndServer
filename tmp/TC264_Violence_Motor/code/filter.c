#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "filter.h"
#include "maths.h"



#define BIQUAD_BANDWIDTH 1.9f     		/* bandwidth in octaves */
#define BIQUAD_Q 1.0f / sqrtf(2.0f)     /* quality factor - butterworth*/

pt1Filter_t Gyro_lowpass;
//一阶低通滤波器
// f_cut = cutoff frequency
void pt1FilterInit(pt1Filter_t *filter, uint8_t f_cut, float dT)
{
    filter->RC = 1.0f / ( 2.0f * M_PIf * f_cut );
    filter->dT = dT;
}

float pt1FilterApply(pt1Filter_t *filter, float input)
{
    filter->state = filter->state + filter->dT / (filter->RC + filter->dT) * (input - filter->state);
    return filter->state;
}

float pt1FilterApply4(pt1Filter_t *filter, float input, uint16_t f_cut, float dT)
{
    // Pre calculate and store RC
    if (!filter->RC) {
        filter->RC = 1.0f / ( 2.0f * M_PIf * f_cut );
    }

    filter->dT = dT;    // cache latest dT for possible use in pt1FilterApply
    filter->state = filter->state + dT / (filter->RC + dT) * (input - filter->state);
    return filter->state;
}

void pt1FilterReset(pt1Filter_t *filter, float input)
{
    filter->state = input;
}

// rate_limit = maximum rate of change of the output value in units per second
void rateLimitFilterInit(rateLimitFilter_t *filter)
{
    filter->state = 0;
}

float rateLimitFilterApply4(rateLimitFilter_t *filter, float input, float rate_limit, float dT)
{
    if (rate_limit > 0) {
        const float rateLimitPerSample = rate_limit * dT;
        filter->state = constrainf(input, filter->state - rateLimitPerSample, filter->state + rateLimitPerSample);
    }
    else {
        filter->state = input;
    }

    return filter->state;
}

float filterGetNotchQ(uint16_t centerFreq, uint16_t cutoff)
{
    const float octaves = log2f((float)centerFreq  / (float)cutoff) * 2;
    return sqrtf(powf(2, octaves)) / (powf(2, octaves) - 1);
}

//二阶陷波器
void biquadFilterInitNotch(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq, uint16_t cutoffHz)
{
    float temp = filterGetNotchQ(filterFreq, cutoffHz);
    biquadFilterInit(filter, samplingFreq, filterFreq, temp, FILTER_NOTCH);
}

//二阶低通滤波器
void biquadFilterInitLPF(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq)
{
    biquadFilterInit(filter, samplingFreq, filterFreq, BIQUAD_Q, FILTER_LPF);
}

//二阶滤波器
void biquadFilterInit(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq, float temp_Q, biquadFilterType_e filterType)
{
    // Check for Nyquist frequency and if it's not possible to initialize filter as requested - set to no filtering at all
    if (filterFreq < (samplingFreq / 2)) {
        // setup variables
        const float sampleRate = samplingFreq;
        const float omega = 2.0f * M_PIf * ((float)filterFreq) / sampleRate;
        const float sn = sin_approx(omega);
        const float cs = cos_approx(omega);
        const float alpha = sn / (2 * temp_Q);

        float b0, b1, b2;
        switch (filterType) {
        case FILTER_LPF:
            b0 = (1 - cs) / 2;
            b1 = 1 - cs;
            b2 = (1 - cs) / 2;
            break;
        case FILTER_NOTCH:
            b0 =  1;
            b1 = -2 * cs;
            b2 =  1;
            break;
        }
        const float a0 =  1 + alpha;
        const float a1 = -2 * cs;
        const float a2 =  1 - alpha;

        // precompute the coefficients
        filter->b0 = b0 / a0;
        filter->b1 = b1 / a0;
        filter->b2 = b2 / a0;
        filter->a1 = a1 / a0;
        filter->a2 = a2 / a0;
    }
    else {
        // Not possible to filter frequencies above Nyquist frequency - passthrough
        filter->b0 = 1.0f;
        filter->b1 = 0.0f;
        filter->b2 = 0.0f;
        filter->a1 = 0.0f;
        filter->a2 = 0.0f;
    }

    // zero initial samples
    filter->d1 = filter->d2 = 0;
}

// Computes a biquad_t filter on a sample
float biquadFilterApply(biquadFilter_t *filter, float input)
{
    const float result = filter->b0 * input + filter->d1;
    filter->d1 = filter->b1 * input - filter->a1 * result + filter->d2;
    filter->d2 = filter->b2 * input - filter->a2 * result;
    return result;
}

/*
 * FIR filter
 */
void firFilterInit2(firFilter_t *filter, float *buf, uint8_t bufLength, const float *coeffs, uint8_t coeffsLength)
{
    filter->buf = buf;
    filter->bufLength = bufLength;
    filter->coeffs = coeffs;
    filter->coeffsLength = coeffsLength;
    memset(filter->buf, 0, sizeof(float) * filter->bufLength);
}

/*
 * FIR filter initialisation
 * If FIR filter is just used for averaging, coeffs can be set to NULL
 */
void firFilterInit(firFilter_t *filter, float *buf, uint8_t bufLength, const float *coeffs)
{
    firFilterInit2(filter, buf, bufLength, coeffs, bufLength);
}

void firFilterUpdate(firFilter_t *filter, float input)
{
    memmove(&filter->buf[1], &filter->buf[0], (filter->bufLength-1) * sizeof(float));
    filter->buf[0] = input;
}

float firFilterApply(const firFilter_t *filter)
{
    float ret = 0.0f;
    for (int ii = 0; ii < filter->coeffsLength; ++ii) {
        ret += filter->coeffs[ii] * filter->buf[ii];
    }
    return ret;
}



/*************************************************************
** Function name:      MCU_Open_interrupt
** Descriptions:       卡尔曼滤波器 函数
** Input parameters:   *kfp    - 卡尔曼结构体参数
** Input parameters:   input   - 需要滤波的参数的测量值（即传感器的采集值）
** Output parameters:  卡尔曼滤波器输出值（最优值）
** Returned value:     卡尔曼滤波器输出值（最优值）
** Created by:         qkk
** Created date:       2024-01-15
*************************************************************/

float KalmanFilter(KFPTypeS_Struct* kfp, float input)
{
    //估算协方差方程：当前 估算协方差 = 上次更新 协方差 + 过程噪声协方差
    kfp->P = kfp->P + kfp->KALMAN_Q;

    //卡尔曼增益方程：当前 卡尔曼增益 = 当前 估算协方差 / （当前 估算协方差 + 测量噪声协方差）
    kfp->G = kfp->P / (kfp->P + kfp->KALMAN_R);

    //更新最优值方程：当前 最优值 = 当前 估算值 + 卡尔曼增益 * （当前 测量值 - 当前 估算值）
    kfp->Output = kfp->Output + kfp->G * (input - kfp->Output); //当前 估算值 = 上次 最优值

    //更新 协方差 = （1 - 卡尔曼增益） * 当前 估算协方差。
    kfp->P = (1 - kfp->G) * kfp->P;

     return kfp->Output;
}
