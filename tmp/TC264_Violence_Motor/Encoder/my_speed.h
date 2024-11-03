/*
 * my_speed.h
 *
 *  Created on: 2024��1��16��
 *      Author: qkk
 */

#ifndef MY_USER_MY_SPEED_H_
#define MY_USER_MY_SPEED_H_
#include "zf_common_headfile.h"
 #include "stdio.h"
 #include "stdint.h"
 #include "string.h"
#include "IfxGpt12.h"
#include "my_speed.h"
#include "my_timer.h"

extern uint32 gMS_SpeedTime;



/*************************************************************
** Function name:      MS_SpeedTime
** Descriptions:       ʱ��
** Input parameters:   �ж�ʱ�䣨��λms��
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
*************************************************************/
#define MS_SpeedTime(ms)                \
        gMS_SpeedTime+=ms


/*************************************************************
** Function name:      GET_SpeedTime
** Descriptions:       ��ȡ��ʼʱ���
** Input parameters:   none
** Output parameters:  none
** Returned value:     (uint32_t)��ʼʱ��
** Created by:         qkk
** Created date:       24/01/16
*************************************************************/
#define GET_SpeedTime(void)                        \
        gMS_SpeedTime

/*************************************************************
** Function name:      RUN_BY_speed_BLOCK
** Descriptions:       ������ִ������
** Input parameters:   ms�����ڣ���λms��
**                     BLOCK��Ҫִ�е�����
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
*************************************************************/
#define RUN_BY_speed_BLOCK(ms, BLOCK)           \
{                                               \
    static uint32_t last_exec_ms = 0;           \
    uint32_t cur_ms_tick = GET_SpeedTime();      \
    if (cur_ms_tick >= (last_exec_ms + ms))     \
    {                                           \
        BLOCK;                                  \
        last_exec_ms = cur_ms_tick;             \
    }                                           \
}

void My_speed_INIT      (void); //��ʼ������
void My_speed_loop      (void); //����1ms���ж���
void My_r_speed_update  (void); //����R��A���ش����ж�
void My_l_speed_update  (void); //����L��A���ش����ж�


#define PI_speed    (3.1415926f)
#define GAIN_speed  (5.0f)//�뾶��5��0cm��/0.001
#define GAIN_ABS   (GAIN_speed*PI_speed*2.0/93.75f*1000.0f)  //104.72
//��λ��cm/s
#define MAX_SPEED_TIMER  (20.0f)


//��������Э����,Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
//��������Э����,R���󣬶�̬��Ӧ�����������ȶ��Ա��
#define Q  (0.010f)
#define R  (0.40f)


struct S_Speed_ab_Struct
{
    int8                dir;
    int8                speed_0;//�ٶȹ����־λ
    float               time;
    float               gain;
    float               abs_speed;
    float               speed;
    float               last_speed;
    float               kal_speed;
    KFPTypeS_Struct     KALMAN_speed;

    STMN_enum           Speed_timer;

    exti_pin_enum       A_pin;
    gpio_pin_enum       a_pin;
    gpio_pin_enum       B_pin;
};
typedef struct S_Speed_ab_Struct Speed_ab_Struct;
typedef Speed_ab_Struct *PSpeed_ab_Struct;



/*************************************************************
** Function name:       Speed_EXPORT
** Descriptions:        ����һ��Speed_ab����
**                      xtimer�����õĶ�ʱ��λ��
**                      xApin��A�ź���
**                      xBpin��B�ź���
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
#define Speed_EXPORT(x,xtimer,xApin,xa_pin,xBpin)      \
Speed_ab_Struct x = {                           \
        .dir = 1,                               \
        .speed_0 = 0,                           \
        .time = 0.0,                            \
        .gain = GAIN_ABS,                       \
        .abs_speed = 0.0,                       \
        .speed = 0.0,                           \
        .last_speed = 0.0,                      \
        .kal_speed=0,                           \
        .KALMAN_speed={0.02,0,Q,R,0.0}, \
        .Speed_timer=xtimer,                    \
        .A_pin=xApin,                           \
        .a_pin=xa_pin,                          \
        .B_pin=xBpin,                           \
}



 void Get_and_start_speed_timer (PSpeed_ab_Struct pSpeed);
 void Get_speed_dir             (PSpeed_ab_Struct pSpeed);
 void Get_speed                 (PSpeed_ab_Struct pSpeed);
 void Get_kalman_speed          (PSpeed_ab_Struct pSpeed);
 void JUDGE_speed_0             (PSpeed_ab_Struct pSpeed);

 #define Speed_EXPORT_extern(x)    Speed_ab_Struct x
 extern  Speed_EXPORT_extern(r_speed);
 extern  Speed_EXPORT_extern(l_speed);


#endif /* MY_USER_MY_SPEED_H_ */
