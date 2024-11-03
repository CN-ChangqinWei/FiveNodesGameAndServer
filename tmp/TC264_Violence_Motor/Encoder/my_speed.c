/*
 * my_speed.c
 *
 *  Created on: 2024��1��16��
 *      Author: qkk
 */

#ifndef MY_USER_MY_SPEED_C_
#define MY_USER_MY_SPEED_C_

#include "my_speed.h"
#include "zf_common_headfile.h"

//������T�����ٲ���
uint32 gMS_SpeedTime=0;

//��������
//Speed_EXPORT(r_speed,STM0,ERU_CH5_REQ1_P15_8,P15_8,P10_1);//A��P15_8B��P0_9
Speed_EXPORT(r_speed,STM0,ERU_CH3_REQ3_P10_3,P10_3,P10_1);
Speed_EXPORT(l_speed,STM1,ERU_CH4_REQ13_P15_5,P20_3,P20_0);//A��P33_7B��P33_6



/*************************************************************
** Function name:      My_speed_INIT
** Descriptions:       T�����ٲ��ٳ�ʼ��
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
** Descriptions:       T������ѭ��
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע����Ҫ���ܰ�����ʱ�����˲���������ѭ����
*************************************************************/
void My_speed_loop(void)
{
    //ѭ��ִ������2ms
    RUN_BY_speed_BLOCK(1,             \
     {
             JUDGE_speed_0(&r_speed);
             //JUDGE_speed_0(&l_speed);

     }
    )
    //ѭ��ִ������3ms
    RUN_BY_speed_BLOCK(2,             \
     {
              Get_kalman_speed(&r_speed);
              //Get_kalman_speed(&l_speed);
     }
    )

}

/*************************************************************
** Function name:      My_r_speed_update
** Descriptions:       T�����ٲ��ٸ�������
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע�������½��ش����ж���
*************************************************************/
void My_r_speed_update(void)
{
    Get_and_start_speed_timer(&r_speed);
    Get_speed_dir(&r_speed);
    Get_speed(&r_speed);
}
/*************************************************************
** Function name:      My_l_speed_update
** Descriptions:       T�����ٲ��ٸ�������
** Input parameters:   none
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע�������½��ش����ж���
*************************************************************/
void My_l_speed_update(void)
{
    Get_and_start_speed_timer(&l_speed);
    Get_speed_dir(&l_speed);
    Get_speed(&l_speed);
}





/*************************************************************
** Function name:      GetElectricalAngle
** Descriptions:       ��ȡʱ�䲢�ҿ�ʼ��ʱ
** Input parameters:   PSpeed_ab_Struct:�ṹ��ָ��
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
** Descriptions:       �õ��ٶȷ���
** Input parameters:   PSpeed_ab_Struct:�ṹ��ָ��
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע��ֻ����A�ж���
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
** Descriptions:       �õ��ٶȷ���
** Input parameters:   PSpeed_ab_Struct:�ṹ��ָ��
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע��ֻ����A�ж���
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
** Descriptions:       �õ��˲��ٶ�
** Input parameters:   PSpeed_ab_Struct:�ṹ��ָ��
** Output parameters:  none
** Returned value:     none
** Created by:         qkk
** Created date:       24/01/16
** ע��3ms�˲�һ��
*************************************************************/
 void Get_kalman_speed(PSpeed_ab_Struct pSpeed)
{
     pSpeed->kal_speed=KalmanFilter(&pSpeed->KALMAN_speed,pSpeed->speed );
}
 /*************************************************************
 ** Function name:      Get_speed_dir
 ** Descriptions:       �õ��ٶȷ���
 ** Input parameters:   PSpeed_ab_Struct:�ṹ��ָ��
 ** Output parameters:  none
 ** Returned value:     none
 ** Created by:         qkk
 ** Created date:       24/01/16
 ** ע����2ms�ж�һ��(������������)
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


