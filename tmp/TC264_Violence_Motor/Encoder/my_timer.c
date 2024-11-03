/*
 * my_timer.c
 *
 *  Created on: 2024��1��30��
 *      Author: qkk
 */

#include "IfxStm.h"
#include "IFXSTM_CFG.h"
#include "zf_driver_timer.h"
#include "my_timer.h"

static uint32 Systick_count[2];



//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick��ʱ������
//  @param      stmn            ѡ��ʹ�õ�ģ��
//  @return     void
//  Sample usage:               systick_start(STM0);//��¼�µ�ǰ��ʱ��
//-------------------------------------------------------------------------------------------------------------------
void systick_start(STMN_enum stmn)
{

    Systick_count[stmn] = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     ��õ�ǰSystem tick timer��ֵ
//  @param     stmn             ѡ��ʹ�õ�ģ��
//  @return    uint32           ���شӿ�ʼ�����ڵ�ʱ��(��λ10ns)
//  Sample usage:               uint32 tim = systick_getval(STM0);
//-------------------------------------------------------------------------------------------------------------------
uint32 systick_getval(STMN_enum stmn)
{
    uint32 time;
    uint32 stm_clk;

    stm_clk = IfxStm_getFrequency(IfxStm_getAddress((IfxStm_Index)stmn));

    time = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn)) - Systick_count[stmn];
    time = (uint32)((uint64)time * 100000000 / stm_clk);
    return time;
}
