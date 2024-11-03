/*
 * Task.c
 *
 *  Created on: 2023Äê12ÔÂ27ÈÕ
 *      Author: Roronoa zoro
 */
#include "Task.h"
#include "Img.h"
TASK_FUNC Task_Actions[_DEF_TASK_NUM]={0};
float Task_Error=0;

float Task_Rocker_Error=0;

int Task_ControlMode=_DEF_TASK_CAMERA;
float   ForwardFoundPWM=0;
float   TurnFoundPWM=0;
float     FFPSign=0;
float     TFPSign=0;
int Task_Index=0;
void Task_Init(){

    Task_Actions[_DEF_TASK_STOP]=Task_Stop;
    Task_Actions[_DEF_TASK_NORMAL]=Task_Normal;
    Task_Actions[_DEF_TASK_PODAO]=Task_Podao;

}
void Task_GenerateRockErr(){

    Task_Rocker_Error=TurnFoundPWM*TFPSign;

}

float Task_GetError(){

    Task_Rocker_Error=TurnFoundPWM*TFPSign;
    if(Task_ControlMode==_DEF_TASK_CAMERA){

        Task_Error=Error;
    }else{

        Task_Error=Task_Rocker_Error;
    }
    return Task_Error;
}
void Task_Normal(){



}
void Task_Podao(){


}
void Task_Stop(){



}
void Task_Excute(){


}
