/*
 * my_timer.c
 *
 *  Created on: 2024年1月30日
 *      Author: qkk
 */

#include "IfxStm.h"
#include "IFXSTM_CFG.h"
#include "zf_driver_timer.h"
#include "my_timer.h"

static uint32 Systick_count[2];



//-------------------------------------------------------------------------------------------------------------------
//  @brief      systick定时器启动
//  @param      stmn            选择使用的模块
//  @return     void
//  Sample usage:               systick_start(STM0);//记录下当前的时间
//-------------------------------------------------------------------------------------------------------------------
void systick_start(STMN_enum stmn)
{

    Systick_count[stmn] = IfxStm_getLower(IfxStm_getAddress((IfxStm_Index)stmn));
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief     获得当前System tick timer的值
//  @param     stmn             选择使用的模块
//  @return    uint32           返回从开始到现在的时间(单位10ns)
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
