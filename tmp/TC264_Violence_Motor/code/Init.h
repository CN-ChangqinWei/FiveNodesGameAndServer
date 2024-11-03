/*
 * Init.h
 *
 *  Created on: 2023年12月26日
 *      Author: Roronoa zoro
 */
//#include "Menu.h"
#ifndef CODE_INIT_H_
#define CODE_INIT_H_
#include "zf_common_headfile.h"


#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)


#define SWITCH1                 (P33_11)
#define SWITCH2                 (P33_12)
#define DIR_R                   (P21_2)
#define DIR_L                   (P21_4)
#define PWM_R                   (ATOM0_CH1_P21_3)
#define PWM_L                   (ATOM0_CH3_P21_5)

#define PWM_LIMT                8000
//******************全局变量定义************************//
extern float VBAT,VBAT_New,VBAT_Last,Motor_Voltage;


void Device_Init(void);
void Algorithms_Init(void);
#endif /* CODE_INIT_H_ */
