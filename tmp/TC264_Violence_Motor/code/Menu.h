/*
 * Menu.h
 *
 *  Created on: 2024Äê3ÔÂ10ÈÕ
 *      Author: howlo
 */

#ifndef CODE_MENU_H_
#define CODE_MENU_H_
#include"key.h"
#include "zf_common_headfile.h"
#include"motor.h"
#include"Img.h"
#include"my_speed.h"
#include"pid.h"



#define _DEF_NOFMENUS           (20)

#define _DEF_MENU_MAIN          (0)

#define _DEF_MENU_VALUEADJUST   (1)

#define _DEF_MENU_ADJUSTGYRO    (2)

#define _DEF_MENU_ADJUSTANGLE   (3)

#define _DEF_MENU_ADJUSTSPEED   (4)

#define _DEF_MENU_ADJUSTSERVO   (5)

#define _DEF_MENU_CONTROLLER    (6)

#define _DEF_MENU_GYROVALUE     (7)

#define _DEF_MENU_FACTORSET     (8)

#define _DEF_MENU_BLTRECV       (9)

#define _DEF_MENU_PHOTO         (10)

#define _DEF_MENU_ADJUSTFORWARD (11)

#define _DEF_MENU_ADJUSTDIRECTION   (12)

#define _FUNC_LIMITCURSOR(min,max,cursor) (cursor<min?max:(cursor>max?min:cursor))

#define _DEF_VOL_FAST           (0)

#define _DEF_VOL_MID            (1)

#define _DEF_VOL_SLOW           (2)

#define _DEF_VOL_NUMS           (3)

#define _DEF_STAT_SELECTING     (0)

#define _DEF_STAT_SETTING       (1)

#define _FUNC_ADJUSTVALUE(PID_Loop)\
    static float* ValueArry[3]={&##PID_Loop##.P,&##PID_Loop##.I,&##PID_Loop##.D};\
            static float ValueTemp[3]={0};\
            static int flag=_DEF_STAT_SELECTING;\
            static int Cursor=0;\
            static float step=1;\
            if(!flag)\
                for(int i=0;i<3;i++) ValueTemp[i]=*ValueArry[i];\
            ips200_show_float(60,20,ValueTemp[0],4,3);\
            ips200_show_float(60,40,ValueTemp[1],4,3);\
            ips200_show_float(60,60,ValueTemp[2],4,3);\
            ips200_show_float(60,80,step,4,3);\
            if(flag){\
                ips200_show_string(0,100,"Setting  ");\
            }else{\
        ips200_show_string(0,100,"Selecting");\
            }\
            static int LastCursor=0;\
                    switch(Key.Trig){\
                        case BUTTON_1:Menu_Action=0;\
                            if(flag){\
                                if(Cursor==3){\
                                    step*=10;\
                                    step=_FUNC_LIMITCURSOR(0.1,100,step);\
                                }else{\
                                    ValueTemp[Cursor]+=step;\
                                }\
                            }else{\
                                Cursor--;\
                            Cursor=_FUNC_LIMITCURSOR(0,3,Cursor);\
                            }\
                            break;\
                        case BUTTON_2:Menu_Action=0;\
                            if(flag){\
                                if(Cursor!=3){\
                                    *ValueArry[Cursor]=ValueTemp[Cursor];\
                                }\
                            }\
                            flag=!flag;\
                            break;\
                        case BUTTON_3:Menu_Action=0;\
                            if(flag){\
                             if(Cursor==3){\
                             step/=10;\
                             step=_FUNC_LIMITCURSOR(1,100,step);\
                           }else{\
                             ValueTemp[Cursor]-=step;\
                             }\
                            }else{\
                            Cursor++;\
                            Cursor=_FUNC_LIMITCURSOR(0,3,Cursor);\
                            }\
                            break;\
                        case BUTTON_4:Menu_Action=0;\
                            if(flag){\
                                flag=!flag;\
                                if(Cursor!=3){\
                                    ValueTemp[Cursor]=*ValueArry[Cursor];\
                                }\
                            }\
                            else\
                            Menu_index=_DEF_MENU_VALUEADJUST;\
                            break;\
                    }\
                    if(LastCursor!=Cursor){\
                        ips200_show_char(20*8,(LastCursor+1)*20,' ');\
                        LastCursor=Cursor;\
                    }\
                    ips200_show_char(20*8,(Cursor+1)*20,'<');




typedef void(*Menu_Func)();

//typedef void(*Menu_FuncValue)(int);



extern int Menu_index;

extern int PhotoShowFlag;

extern Menu_Func Menus[_DEF_NOFMENUS];

extern Menu_Func Menus_Refresh[_DEF_NOFMENUS];

extern int Menu_KeyReset;

extern uint8 Menu_Action;

void Menu_Init();
void Menu_Proccess();

void Menu_Main();

void Menu_ValueAdjust();

void Menu_AdjustGyro();

void Menu_AdjustAngle();

void Menu_AdjustSpeed();

void Menu_AdjustServo();

void Menu_ControlMotorAndServo();

void Menu_GyroValue();

void Menu_FactorSet();

void Menu_BLTRecv();

void Menu_Photo();

void Menu_AdjustForward();

void Menu_AdjustDirection();

void MenuRefresh_Main();

void MenuRefresh_ValueAdjust();

void MenuRefresh_AdjustGyro();

void MenuRefresh_AdjustAngle();

void MenuRefresh_AdjustSpeed();

void MenuRefresh_AdjustServo();

void MenuRefresh_ControlMotorAndServo();

void MenuRefresh_GyroValue();

void MenuRefresh_FactorSet();

void MenuRefresh_BLTRecv();

void MenuRefresh_Photo();

void MenuRefresh_AdjustForward();

void MenuRefresh_Direction();

#endif /* CODE_MENU_H_ */
