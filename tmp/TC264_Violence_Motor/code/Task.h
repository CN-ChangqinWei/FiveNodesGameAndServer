/*
 * Task.h
 *
 *  Created on: 2023Äê12ÔÂ27ÈÕ
 *      Author: Roronoa zoro
 */

#ifndef CODE_TASK_H_
#define CODE_TASK_H_
#include "zf_common_headfile.h"
#define _DEF_TASK_CAMERA (0)
#define _DEF_TASK_ROCKER (1)
#define _DEF_TASK_STOP  (0)
#define _DEF_TASK_NORMAL (1)
#define _DEF_TASK_PODAO (2)
#define _DEF_TASK_NUM   (4)
typedef void(*TASK_FUNC)();
extern float   ForwardFoundPWM;
extern float   TurnFoundPWM;
extern float     FFPSign;
extern float     TFPSign;
extern float Task_Error;
extern float Task_Rocker_Error;
extern int Task_ControlMode;
extern int Task_Index;
float Task_GetError();

void Task_Normal();
void Task_Podao();
void Task_Stop();
void Task_Excute();
void Task_Init();

#endif /* CODE_TASK_H_ */
