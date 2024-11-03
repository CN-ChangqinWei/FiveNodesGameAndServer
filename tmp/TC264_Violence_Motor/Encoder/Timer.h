/********************** Copyright(c)***************************
** Shenzhen Yuejiang Technology Co., Ltd.
**
** http:// www.dobot.cc
**
**------File Info----------------------------------------------
** File name:               IODriver.h
** Latest modified date:    2020-04-22
** Latest version:          V1.0.0
** Description:             系统时钟
**
**------------------------------------------------------------
** Created by:              liu xiang
** Created date:            2020-04-22
** Version:                 V1.0.0
** Descriptions:            系统时钟
**
**------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Description:
**
*************************************************************/
#ifndef __TIMER__H
#define __TIMER__H
//#include "headfile.h"
//#include "zf_common_headfile.h"

#include "stdio.h"
#include "stdint.h"
#include "string.h"
extern uint32_t gMS_TimeCNT;

/*************************************************************
** Function name:       MS_TIMEBASE
** Descriptions:        时基,放在周期为1ms的函数里面执行
** Input parameters:    none
** Output parameters:   none
** Returned value:      none
** ** Created by:          qkk
** Created date:        24/01/16
*************************************************************/
#define MS_TIMEBASE(ms)                \
        gMS_TimeCNT+=ms

/*************************************************************
** Function name:       MS_GETTIME
** Descriptions:        获取起始时间
** Input parameters:    none
** Input parameters:    none
** Returned value:      (uint32_t)起始时间
** Created by:          qkk
** Created date:        24/01/16
*************************************************************/
#define MS_GETTIME(void)                        \
        gMS_TimeCNT

/*************************************************************
** Function name:       RUN_BY_LIMIT_BLOCK
** Descriptions:        周期性执行任务
** Input parameters:    ms：周期（单位ms）
**                      BLOCK：要执行的任务
** Output parameters:   None
** Returned value:      None
** Created by:          qkk
** Created date:        24/01/16
*************************************************************/
#define RUN_BY_LIMIT_BLOCK(ms, BLOCK)           \
{                                               \
    static uint32_t last_exec_ms = 0;           \
    uint32_t cur_ms_tick = MS_GETTIME();   \
    if (cur_ms_tick >= (last_exec_ms + ms))     \
    {                                           \
        BLOCK;                                  \
        last_exec_ms = cur_ms_tick;             \
    }                                           \
}

#endif


