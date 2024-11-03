/*
 * my_speed.c
 *
 *  Created on: 2024年1月16日
 *      Author: qkk
 */

#ifndef MY_USER_MY_SPEED_C_
#define MY_USER_MY_SPEED_C_

#include "my_speed.h"
#include "zf_common_headfile.h"

//编码器T法测速部分
uint32 gMS_SpeedTime=0;

//声明对象
//Speed_EXPORT(r_speed,STM0,ERU_CH5_REQ1_P15_8,P15_8,P10_1);//A相P15_8B相P0_9
Speed_EXPORT(r_speed,STM0,ERU_CH3_REQ3_P10_3,P10_3,P10_1);
Speed_EXPORT(l_speed,STM1,ERU_CH4_REQ13_P15_5,P20_3,P20_0);//A相P33_7B相P33_6



/*************************************************************
** Function name:      My_speed_INIT
** Descriptions:       T法测速测速初始化
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
*************************************************************/
void My_speed_INIT(void)
{
    exti_init(r_speed.A_pin,EXTI_TRIGGER_FALLING);
    gpio_init(r_speed.B_pin, GPI, GPIO_LOW, GPI_FLOATING_IN);
//    exti_init(l_speed.A_pin,EXTI_TRIGGER_FALLING);
//    gpio_init(l_speed.B_pin, GPI, GPIO_LOW, GPI_FLOATING_IN);
    Get_and_start_speed_timer(&r_speed);
   // Get_and_start_speed_timer(&l_speed);

}

/*************************************************************
** Function name:      My_speed_loop
** Descriptions:       T法测速循环
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：主要功能包括超时检测和滤波，放在主循环中
*************************************************************/
void My_speed_loop(void)
{
    //循环执行周期2ms
    RUN_BY_speed_BLOCK(1,             \
     {
             JUDGE_speed_0(&r_speed);
             //JUDGE_speed_0(&l_speed);

     }
    )
    //循环执行周期3ms
    RUN_BY_speed_BLOCK(2,             \
     {
              Get_kalman_speed(&r_speed);
              //Get_kalman_speed(&l_speed);
     }
    )

}

/*************************************************************
** Function name:      My_r_speed_update
** Descriptions:       T法测速测速更新数据
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：放在下降沿触发中断中
*************************************************************/
void My_r_speed_update(void)
{
    Get_and_start_speed_timer(&r_speed);
    Get_speed_dir(&r_speed);
    Get_speed(&r_speed);
}
/*************************************************************
** Function name:      My_l_speed_update
** Descriptions:       T法测速测速更新数据
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：放在下降沿触发中断中
*************************************************************/
void My_l_speed_update(void)
{
    Get_and_start_speed_timer(&l_speed);
    Get_speed_dir(&l_speed);
    Get_speed(&l_speed);
}





/*************************************************************
** Function name:      GetElectricalAngle
** Descriptions:       获取时间并且开始计时
** Input parameters:   PSpeed_ab_Struct:结构体指针
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
*************************************************************/
void Get_and_start_speed_timer(PSpeed_ab_Struct pSpeed)
{
   pSpeed->time =Systick_getval_ms(pSpeed->Speed_timer);
   systick_start(pSpeed->Speed_timer);

}
/*************************************************************
** Function name:      Get_speed_dir
** Descriptions:       得到速度方向
** Input parameters:   PSpeed_ab_Struct:结构体指针
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：只放在A中断中
*************************************************************/
void Get_speed_dir(PSpeed_ab_Struct pSpeed)
{
  int8 apin=0,bpin=0;
  bpin=gpio_get_level(pSpeed->B_pin);
  apin=gpio_get_level(pSpeed->a_pin);
  if(apin==bpin)pSpeed->dir=-1;
  else pSpeed->dir=1;

}
/*************************************************************
** Function name:      Get_speed_dir
** Descriptions:       得到速度方向
** Input parameters:   PSpeed_ab_Struct:结构体指针
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：只放在A中断中
*************************************************************/
void Get_speed(PSpeed_ab_Struct pSpeed)
{
   if(pSpeed->speed_0 == 1)
   {
       pSpeed->abs_speed=0;
       pSpeed->speed_0 = 0;
   }else {
       pSpeed->abs_speed=pSpeed->gain*(1.0/(pSpeed->time+0.0001)-1.0/MAX_SPEED_TIMER);
   }

   pSpeed->speed = pSpeed->abs_speed*pSpeed->dir;

   if(pSpeed->speed > pSpeed->last_speed+10.0)
   {
       pSpeed->speed  = pSpeed->last_speed+10.0;
   }
   else if(pSpeed->speed < pSpeed->last_speed-10.0)
   {
       pSpeed->speed   = pSpeed->last_speed-10.0;
   }
   pSpeed->last_speed  = pSpeed->speed;

}

/*************************************************************
** Function name:      Get_kalman_speed
** Descriptions:       得到滤波速度
** Input parameters:   PSpeed_ab_Struct:结构体指针
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** 注：3ms滤波一次
*************************************************************/
 void Get_kalman_speed(PSpeed_ab_Struct pSpeed)
{
     pSpeed->kal_speed=KalmanFilter(&pSpeed->KALMAN_speed,pSpeed->speed );
}
 /*************************************************************
 ** Function name:      Get_speed_dir
 ** Descriptions:       得到速度方向
 ** Input parameters:   PSpeed_ab_Struct:结构体指针
 ** Output parameters:  none
 ** Returned value:     none
 ** Created by:         qkk
 ** Created date:       24/01/16
 ** 注：请2ms判断一次(放在主函数中)
 *************************************************************/
  void JUDGE_speed_0(PSpeed_ab_Struct pSpeed)
 {
      pSpeed->time =Systick_getval_ms(pSpeed->Speed_timer);
      if(pSpeed->time > MAX_SPEED_TIMER)
          {
              pSpeed->speed_0=1;
              pSpeed->speed  =0;
              systick_start(pSpeed->Speed_timer);
          }
 }


#endif /* MY_USER_MY_SPEED_C_ */


