/*
 * motor.c
 *
 *  Created on: 2023��12��29��
 *      Author: Roronoa zoro
 */
#include "motor.h"

Motor_PWM   Motor;
float Current_L,Current_R,Current_L_bias,Current_R_bias;
void Set_PWM(Motor_PWM* motor)
{
    //�޷�
    motor->duty_L=func_limit(motor->duty_L,motor->limt);
    motor->duty_R=func_limit(motor->duty_R,motor->limt);

    if(motor->brake)
    {
        pwm_set_duty(PWM_L,0);                   // ����ռ�ձ�
        motor->duty_L=0;
        pwm_set_duty(PWM_R,0);                   // ����ռ�ձ�
        motor->duty_R=0;
        return;
    }

    if(motor->duty_L>0)
    {
        gpio_set_level(DIR_L, GPIO_HIGH);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_L, motor->duty_L);                   // ����ռ�ձ�
    }
    if(motor->duty_L<0)
    {
        gpio_set_level(DIR_L, GPIO_LOW);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_L, -motor->duty_L);                   // ����ռ�ձ�
    }

    if(motor->duty_R>0)
    {
        gpio_set_level(DIR_R, GPIO_HIGH);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_R, motor->duty_R);                   // ����ռ�ձ�
    }
    if(motor->duty_R<0)
    {
        gpio_set_level(DIR_R, GPIO_LOW);                                   // DIR����ߵ�ƽ
        pwm_set_duty(PWM_R, -motor->duty_R);                   // ����ռ�ձ�
    }
}

void Test_Motor(void)
{
    static int16 Duty=0,Dir=0;
    if(Duty>=2000)Dir=0;//����
    else if(Duty<-2000)Dir=1;
    if(Dir)Duty++;
    else Duty--;
    Motor.duty_R=Motor.duty_L=Duty;
    Set_PWM(&Motor);
    system_delay_ms(2);
}

