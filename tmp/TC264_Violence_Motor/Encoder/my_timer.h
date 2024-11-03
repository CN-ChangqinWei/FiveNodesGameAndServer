/*
 * my_timer.h
 *
 *  Created on: 2024��1��30��
 *      Author: qkk
 */

#ifndef CODE_MY_TIMER_H_
#define CODE_MY_TIMER_H_


#include "zf_common_typedef.h"
 #include "stdio.h"
 #include "stdint.h"
 #include "string.h"



typedef enum  // ö��STMģ���
{
    STM0,
    STM1,
}STMN_enum;



void    systick_start   (STMN_enum stmn);
uint32  systick_getval  (STMN_enum stmn);

//------------------------------------���º궨�����ڻ�ȡ��ǰʱ��------------------------------------
#define Systick_getval_ms(stmn)         systick_getval(stmn)/100000         //��ȡ��ǰ��ʱʱ��  ��λms
#define Systick_getval_us(stmn)         systick_getval(stmn)/100            //��ȡ��ǰ��ʱʱ��  ��λus
#define Systick_getval_ns(stmn)         systick_getval(stmn)*10             //��ȡ��ǰ��ʱʱ��  ��λns



#endif /* CODE_MY_TIMER_H_ */
