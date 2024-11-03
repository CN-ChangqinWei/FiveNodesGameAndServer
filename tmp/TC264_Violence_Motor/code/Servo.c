/*
 * Servo.c
 *
 *  Created on: 2024Äê1ÔÂ30ÈÕ
 *      Author: Roronoa zoro
 */
#include "Servo.h"

#define Mid_PWM 750

Servo_typedef Servo={
        .Servo_MidPWM=Mid_PWM,
        .Servo_PWM_Out=Mid_PWM,
        .Servo_PWM_Input=Mid_PWM,
};

void Servo_Init(void)
{
    pwm_init(Servo_PWM_Pin, 50, Mid_PWM);
}

void Servo_SetPWM(int PWM)
{

    pwm_set_duty(Servo_PWM_Pin,PWM);
}
