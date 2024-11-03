/*
 * my_timer.h
 *
 *  Created on: 2024年1月30日
 *      Author: qkk
 */

#ifndef CODE_MY_TIMER_H_
#define CODE_MY_TIMER_H_


#include "zf_common_typedef.h"
 #include "stdio.h"
 #include "stdint.h"
 #include "string.h"



typedef enum  // 枚举STM模块号
{
    STM0,
    STM1,
}STMN_enum;



void    systick_start   (STMN_enum stmn);
uint32  systick_getval  (STMN_enum stmn);

//------------------------------------以下宏定义用于获取当前时间------------------------------------
#define Systick_getval_ms(stmn)         systick_getval(stmn)/100000         //获取当前计时时间  单位ms
#define Systick_getval_us(stmn)         systick_getval(stmn)/100            //获取当前计时时间  单位us
#define Systick_getval_ns(stmn)         systick_getval(stmn)*10             //获取当前计时时间  单位ns



#endif /* CODE_MY_TIMER_H_ */
