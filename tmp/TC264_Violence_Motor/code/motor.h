/*
 * motor.h
 *
 *  Created on: 2023Äê12ÔÂ29ÈÕ
 *      Author: Roronoa zoro
 */

#ifndef CODE_MOTOR_H_
#define CODE_MOTOR_H_
#include "zf_common_headfile.h"
typedef struct
{
        uint8   brake;
        uint8   L_dir;
        uint8   R_dir;
        int     duty_L;
        int     duty_R;
        int     limt;
}Motor_PWM;
extern Motor_PWM   Motor;
extern float Current_L,Current_R,Current_L_bias,Current_R_bias;
void Set_PWM(Motor_PWM* motor);
void Test_Motor(void);
#endif /* CODE_MOTOR_H_ */
