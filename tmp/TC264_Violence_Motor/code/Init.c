/*
 * Init.c
 *
 *  Created on: 2023锟斤拷12锟斤拷26锟斤拷
 *      Author: Roronoa zoro
 */

#include "Init.h"
#include"pid.h"
#include"key.h"

int Duty_R,Duty_L;

void LED_Init(void)
{
//    gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // 锟斤拷始锟斤拷 LED1 锟斤拷锟� 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟侥Ｊ�
//    gpio_init(LED2, GPO, GPIO_HIGH, GPO_PUSH_PULL);         // 锟斤拷始锟斤拷 LED2 锟斤拷锟� 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟侥Ｊ�
    //gpio_init(LED1, GPO, GPIO_LOW, GPO_PUSH_PULL);          // 锟斤拷始锟斤拷 LED3 锟斤拷锟� 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟侥Ｊ�
   // gpio_init(LED2, GPO, GPIO_LOW, GPO_PUSH_PULL);         // 锟斤拷始锟斤拷 LED4 锟斤拷锟� 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟侥Ｊ�


}

void Key_Init(void)
{
    gpio_init(P33_10, GPI, GPIO_HIGH, GPI_PULL_UP);           // 锟斤拷始锟斤拷 KEY1 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_11, GPI, GPIO_HIGH, GPI_PULL_UP);           // 锟斤拷始锟斤拷 KEY2 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_12, GPI, GPIO_HIGH, GPI_PULL_UP);           // 锟斤拷始锟斤拷 KEY3 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_13, GPI, GPIO_HIGH, GPI_PULL_UP);           // 锟斤拷始锟斤拷 KEY4 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷

    gpio_init(P33_6, GPI, GPIO_HIGH, GPI_PULL_UP);        // 锟斤拷始锟斤拷 SWITCH1 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_7, GPI, GPIO_HIGH, GPI_PULL_UP);        // 锟斤拷始锟斤拷 SWITCH2 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_8, GPI, GPIO_HIGH, GPI_PULL_UP);        // 锟斤拷始锟斤拷 SWITCH1 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    gpio_init(P33_9, GPI, GPIO_HIGH, GPI_PULL_UP);        // 锟斤拷始锟斤拷 SWITCH2 锟斤拷锟斤拷 默锟较高碉拷平 锟斤拷锟斤拷锟斤拷锟斤拷
    MotorVols[0].Normal=_DEF_VOL_NORMAL;
    MotorVols[0].Cross=_DEF_VOL_CROSS;
    MotorVols[0].Island=_DEF_VOL_ISLAND;
    MotorVols[0].Bend=_DEF_VOL_BEND;

    MotorVols[1].Normal=_DEF_VOL_NORMAL_MID;
    MotorVols[1].Cross=_DEF_VOL_CROSS_MID;
    MotorVols[1].Island=_DEF_VOL_ISLAND_MID;
    MotorVols[1].Bend=_DEF_VOL_BEND_MID;

    MotorVols[2].Normal=_DEF_VOL_NORMAL_SLOW;
    MotorVols[2].Cross=_DEF_VOL_CROSS_SLOW;
    MotorVols[2].Island=_DEF_VOL_ISLAND_SLOW;
    MotorVols[2].Bend=_DEF_VOL_BEND_SLOW;

}
void IPS2_0_Init(void)
{
    ips200_set_dir(IPS200_PORTAIT);
    ips200_set_color(RGB565_RED, RGB565_GREEN);
    ips200_init(IPS200_TYPE_PARALLEL8);
    ips200_draw_line(IPS_X_MAX/2,0,IPS_X_MAX/2,IPS_Y_MAX-1,RGB565_RED);
}
void Motor_Init(void)
{
    gpio_init(DIR_R, GPO, GPIO_LOW, GPO_PUSH_PULL);                            // GPIO 锟斤拷始锟斤拷为锟斤拷锟� 默锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
    pwm_init(PWM_R, 1000, 0);                                                  // PWM 通锟斤拷锟斤拷始锟斤拷频锟斤拷 17KHz 占锟秸比筹拷始为 0
    gpio_init(DIR_L, GPO, GPIO_LOW, GPO_PUSH_PULL);                            // GPIO 锟斤拷始锟斤拷为锟斤拷锟� 默锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟�
    pwm_init(PWM_L, 1000, 0);                                                  // PWM 通锟斤拷锟斤拷始锟斤拷频锟斤拷 17KHz 占锟秸比筹拷始为 0
    Motor.limt=10000;                                                            //12V锟斤拷锟斤拷薹锟�50%
    Motor.brake=1;
}

void IMU_Calibrate(void)
{
    uint16 i=0;
    for (int axis = 0; axis < 3; axis++)
    {
        biquadFilterInitLPF(&gyroFilterLPF[axis], 500, 100.0f);
    }
    for (int axis = 0; axis < 3; axis++)
    {
        biquadFilterInitLPF(&accFilterLPF[axis], 500, 15.0f);
    }

    for(i=0;i<500;i++)
    {
        IMU_Get_gyro();
        gyrof_bias.z+=gyro_transition(gyro_z);
        gyrof_bias.x+=gyro_transition(gyro_x);
        gyrof_bias.y+=gyro_transition(gyro_y);
       system_delay_ms(2);
    }
    gyrof_bias.z/=500;
    gyrof_bias.x/=500;
    gyrof_bias.y/=500;
}
float VBAT=0,VBAT_New=0,VBAT_Last=0,Motor_Voltage=3.4;
void Device_Init(void)
{

    LED_Init();
    Vofa_uart_init(115200,bluetooth);
    uart_init(UART_3,3000000,UART3_TX_P15_6,UART3_RX_P15_7);
    while(IMU_Init());
    IMU_Calibrate();
    Motor_Init();

//    while(1)
//    {
//        if(dl1a_init())
//           gpio_toggle_level(LED1);                                            // ��ת LED ���������ƽ ���� LED ���� ��ʼ����������ƻ����ĺ���
//        else
//           break;
//        system_delay_ms(1000);                                                  // ���Ʊ�ʾ�쳣
//    }
    adc_init(ADC0_CH8_A8,ADC_12BIT);
    VBAT_Last=adc_mean_filter_convert(ADC0_CH8_A8,100)*1.0f/4096.0f*69.7f-0.04f;
    Key_Init();
    Servo_Init();
    pit_ms_init(CCU60_CH0, 1);//CCU60_CH0锟斤拷时1ms
    pit_ms_init(CCU60_CH1,10);//CCU60_CH0锟斤拷时10ms
}

void Algorithms_Init(void)
{

    pt1FilterInit(&Gyro_lowpass,2,0.002);
    PID_Param_Init();
}
