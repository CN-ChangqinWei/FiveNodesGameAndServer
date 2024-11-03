#ifndef __FILTER_H
#define __FILTER_H



typedef struct rateLimitFilter_s {
    float state;
} rateLimitFilter_t;

typedef struct pt1Filter_s {
    float state;
    float RC;
    float dT;
} pt1Filter_t;

/* this holds the data required to update samples thru a filter */
typedef struct biquadFilter_s {
    float b0, b1, b2, a1, a2;
    float d1, d2;
} biquadFilter_t;

typedef enum {
    FILTER_PT1 = 0,
    FILTER_BIQUAD,
    FILTER_FIR,
} filterType_e;

typedef enum {
    FILTER_LPF,
    FILTER_NOTCH
} biquadFilterType_e;

typedef union
{
    struct
    {
        float x;
        float y;
        float z;
    };
    float axis[3];
} Axis3f;
typedef struct firFilter_s {
    float *buf;
    const float *coeffs;
    uint8_t bufLength;
    uint8_t coeffsLength;
} firFilter_t;





//1. 结构体类型定义
typedef struct
{
    float P; //估算协方差
    float G; //卡尔曼增益
    float KALMAN_Q; //过程噪声协方差,Q增大，动态响应变快，收敛稳定性变坏
    float KALMAN_R; //测量噪声协方差,R增大，动态响应变慢，收敛稳定性变好
    float Output; //卡尔曼滤波器输出
}KFPTypeS_Struct; //Kalman Filter parameter type Struct

float KalmanFilter(KFPTypeS_Struct* kfp, float input);

void pt1FilterInit(pt1Filter_t *filter, uint8_t f_cut, float dT);
float pt1FilterApply(pt1Filter_t *filter, float input);
float pt1FilterApply4(pt1Filter_t *filter, float input, uint16_t f_cut, float dt);
void pt1FilterReset(pt1Filter_t *filter, float input);

void rateLimitFilterInit(rateLimitFilter_t *filter);
float rateLimitFilterApply4(rateLimitFilter_t *filter, float input, float rate_limit, float dT);

void biquadFilterInitNotch(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq, uint16_t cutoffHz);
void biquadFilterInitLPF(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq);
void biquadFilterInit(biquadFilter_t *filter, uint16_t samplingFreq, uint16_t filterFreq, float tempQ, biquadFilterType_e filterType);
float biquadFilterApply(biquadFilter_t *filter, float sample);
float filterGetNotchQ(uint16_t centerFreq, uint16_t cutoff);

void firFilterInit(firFilter_t *filter, float *buf, uint8_t bufLength, const float *coeffs);
void firFilterInit2(firFilter_t *filter, float *buf, uint8_t bufLength, const float *coeffs, uint8_t coeffsLength);
void firFilterUpdate(firFilter_t *filter, float input);
float firFilterApply(const firFilter_t *filter);
extern pt1Filter_t Gyro_lowpass;
#endif //__FILTER_H

