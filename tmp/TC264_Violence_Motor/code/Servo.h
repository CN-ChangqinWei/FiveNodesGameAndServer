/*
 * Servo.h
 *
 *  Created on: 2024年1月30日
 *      Author: Roronoa zoro
 */

#ifndef CODE_SERVO_H_
#define CODE_SERVO_H_
#include "zf_common_headfile.h"

/******舵机角度范围*******/
#define Min_Angle 50
#define Mid_Angle 85
#define Max_Angle 120
#define Mid_PWM 750
#define Servo_PWM_Pin   ATOM0_CH5_P32_4
/******舵机角度范围*******/

typedef struct
{
       uint16   Servo_MidPWM;        //机械零点对应的PWM
       float    Servo_PWM_Out;        //输出的舵机PWM
       float    Servo_PWM_Input;

}Servo_typedef;

extern Servo_typedef Servo;

void Servo_Init(void);
void Servo_SetPWM(int PWM);
#endif /* CODE_SERVO_H_ */
