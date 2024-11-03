/*
 * Menu.c
 *
 *  Created on: 2024Äê3ÔÂ10ÈÕ
 *      Author: howlo
 */
#include"Menu.h"
#include"Task.h"
#include"key.h"
#include"pid.h"
int Menu_index=0;
int PhotoShowFlag=0;
Menu_Func Menus[_DEF_NOFMENUS]={0};
Menu_Func Menus_Refresh[_DEF_NOFMENUS]={0};
uint8 Menu_Action=0;
int Menu_KeyReset=0;
Voltages Vols[_DEF_VOL_NUMS];
void Menu_Keys(){
    if(Keys[0]!=LastKeys[0]||Keys[1]!=LastKeys[1]||Keys[2]!=LastKeys[2]||Keys[3]!=LastKeys[3]){
        memcpy(LastKeys,Keys,sizeof(Keys));
        if(Keys[0]&&Keys[1]&&Keys[2]&&Keys[3]){
            Motor.brake=0;
            Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=ServoBalance_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag=1;
            ImgShowFlag=1;Menu_index=_DEF_MENU_PHOTO;
            RoadStat=FactorStat=0;
            Menu_Proccess();
            return;
        }
        Motor.brake=1;
        Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=ServoBalance_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag=0;
        Gyro_loop.Integral=Direction_loop.Integral=Angle_loop.Integral=ServoBalance_loop.Integral=0;
        RoadStat=FactorStat=0;
        if(Keys[3]){
        ImgShowFlag=1;Menu_index=_DEF_MENU_PHOTO;
        Menu_Proccess();
        return;

        }
        if(Keys[2]){

            Menu_index=_DEF_MENU_GYROVALUE;
            Menu_Proccess();
            return;

        }
        Menu_index=_DEF_MENU_MAIN;
        Menu_Proccess();
        //if(Key)
    }





}

void Menu_Init(){

    Menus[_DEF_MENU_MAIN]=Menu_Main;
    Menus[_DEF_MENU_VALUEADJUST]=Menu_ValueAdjust;
    Menus[_DEF_MENU_ADJUSTGYRO]=Menu_AdjustGyro;
    Menus[_DEF_MENU_ADJUSTANGLE]=Menu_AdjustAngle;
    Menus[_DEF_MENU_ADJUSTSPEED]=Menu_AdjustSpeed;
    Menus[_DEF_MENU_ADJUSTSERVO]=Menu_AdjustServo;
    Menus[_DEF_MENU_CONTROLLER]=Menu_ControlMotorAndServo;
    Menus[_DEF_MENU_GYROVALUE]=Menu_GyroValue;
    Menus[_DEF_MENU_FACTORSET]=Menu_FactorSet;
    Menus[_DEF_MENU_BLTRECV]=Menu_BLTRecv;
    Menus[_DEF_MENU_PHOTO]=Menu_Photo;
    Menus[_DEF_MENU_ADJUSTFORWARD]=Menu_AdjustForward;
    Menus[_DEF_MENU_ADJUSTDIRECTION]=Menu_AdjustDirection;

    Menus_Refresh[_DEF_MENU_MAIN]=MenuRefresh_Main;
    Menus_Refresh[_DEF_MENU_VALUEADJUST]=MenuRefresh_ValueAdjust;
    Menus_Refresh[_DEF_MENU_ADJUSTGYRO]=MenuRefresh_AdjustGyro;
    Menus_Refresh[_DEF_MENU_ADJUSTANGLE]=MenuRefresh_AdjustAngle;
    Menus_Refresh[_DEF_MENU_ADJUSTSPEED]=MenuRefresh_AdjustSpeed;
        Menus_Refresh[_DEF_MENU_ADJUSTSERVO]=MenuRefresh_AdjustServo;
        Menus_Refresh[_DEF_MENU_CONTROLLER]=MenuRefresh_ControlMotorAndServo;
        Menus_Refresh[_DEF_MENU_GYROVALUE]=MenuRefresh_GyroValue;
        Menus_Refresh[_DEF_MENU_FACTORSET]=MenuRefresh_FactorSet;
        Menus_Refresh[_DEF_MENU_BLTRECV]=MenuRefresh_BLTRecv;
        Menus_Refresh[_DEF_MENU_PHOTO]=MenuRefresh_Photo;
    Menus_Refresh[_DEF_MENU_ADJUSTFORWARD]=MenuRefresh_AdjustForward;
    Menus_Refresh[_DEF_MENU_ADJUSTDIRECTION]=MenuRefresh_Direction;


    (*Menus_Refresh[_DEF_MENU_MAIN])();
}

void Menu_Proccess(){
    static int LastIndex=_DEF_MENU_MAIN;
    if(Menu_index!=LastIndex){
        LastIndex=Menu_index;
        ips200_clear();
        if(Menu_index<_DEF_NOFMENUS){
           (*Menus_Refresh[Menu_index])();
        }
    }
    if(Menu_index<_DEF_NOFMENUS)
        (*Menus[Menu_index])();
    Menu_Action=0;
}

void Menu_Main(){
    static int Flag=0;
    static int MenuIndex[6]={
            _DEF_MENU_VALUEADJUST,
            _DEF_MENU_CONTROLLER,
            _DEF_MENU_GYROVALUE,
            _DEF_MENU_FACTORSET,
            _DEF_MENU_BLTRECV,
            _DEF_MENU_PHOTO
    };
    static int Cursor=0;
    static int LastCursor=0;

    if(Motor.brake==0){
        ips200_show_string(0,120,"Runing");

    }else{

        ips200_show_string(0,120,"Stop  ");

    }
    ips200_show_float(0,140,euler_deg.Roll,3,3);
//    ips200_show_float(0,140,Direction_loop.OutPut,3,3);
//        ips200_show_float(0,160,Direction_loop.P,3,3);
//        ips200_show_float(0,180,Direction_loop.Error,3,3);
        ips200_show_float(0,200,Error,3,3);
        ips200_show_int(0,220,Keys[0],3);
        ips200_show_int(20,220,Keys[1],3);
        ips200_show_int(40,220,Keys[2],3);
        ips200_show_int(60,220,Keys[3],3);
//
//        ips200_show_int(0,220,Task_ControlMode,3);
//        ips200_show_float(0,240,FFPSign,3,3);
//        ips200_show_float(100,240,TFPSign,3,3);
//        ips200_show_float(0,260,ForwardFoundPWM,3,3);
//        ips200_show_float(100,260,TurnFoundPWM,3,3);
//        ips200_show_float(0,280,Encoder_Sum,4,3);
//        ips200_show_float(60,280,Encoder_Diff,4,3);
//        ips200_show_int(0,300,encoder.L_Dat,4);
//        ips200_show_int(60,300,encoder.R_Dat,4);
    switch(Menu_Action){
        case BUTTON_1:
            Cursor--;
            Cursor=_FUNC_LIMITCURSOR(0,6,Cursor);
            Menu_Action=0;
            break;
        case BUTTON_2:
            if(Cursor!=6)
            Menu_index=MenuIndex[Cursor];
            else{
                //StartFlag=!StartFlag;
                if(Flag==0){

                    Gyro_loop.Iout=Angle_loop.Iout=Direction_loop.Iout=0;
                    Motor.brake=0;
                    Gyro_loop.out_flag=Angle_loop.out_flag=ServoBalance_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag=1;
                    //ImgShowFlag=1;Menu_index=_DEF_MENU_PHOTO;
                    RoadStat=FactorStat=0;
                    StartTime=0;
                    Flag=1;
                }else if(Flag==1){
                    Direction_loop.out_flag=1;
                    RoadStat=FactorStat=0;
                    ForwardFlag=1;
                    Flag=2;
                    StartTime=0;
                    ImgShowFlag=1;
                    Menu_index=_DEF_MENU_PHOTO;
                }else{
                     Motor.brake=1;
                     Gyro_loop.Iout=Angle_loop.Iout=Direction_loop.Iout=0;
                    Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=ServoBalance_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag=0;
                    RoadStat=FactorStat=0;
                    ForwardFlag=0;
                    Flag=0;
                    StartTime=0;
                }
                }Menu_Action=0;
            break;

        case BUTTON_3:
            Cursor++;
            Cursor=_FUNC_LIMITCURSOR(0,6,Cursor);Menu_Action=0;
            break;
        case BUTTON_4:
            Menu_Action=0;
            break;
    }
//ips200_clear_region(0,11*8,11*8+16,200);
    if(LastCursor!=Cursor){
        ips200_show_char(11*8,LastCursor*20,' ');
        LastCursor=Cursor;
    }
    ips200_show_char(11*8,Cursor*20,'<');
}

void Menu_ValueAdjust(){


    static int MenuIndex[6]={
            _DEF_MENU_ADJUSTGYRO,
            _DEF_MENU_ADJUSTANGLE,
            _DEF_MENU_ADJUSTSPEED,
            _DEF_MENU_ADJUSTSERVO,
            _DEF_MENU_ADJUSTFORWARD,
            _DEF_MENU_ADJUSTDIRECTION
       };
    static int Cursor=0;
        static int LastCursor=0;
        switch(Menu_Action){
            case BUTTON_1:
                Cursor--;
                Cursor=_FUNC_LIMITCURSOR(0,5,Cursor);
                Menu_Action=0;
                break;
            case BUTTON_2:

                Menu_index=MenuIndex[Cursor];
                Menu_Action=0;
                break;

            case BUTTON_3:
                Cursor++;
                Cursor=_FUNC_LIMITCURSOR(0,5,Cursor);Menu_Action=0;
                break;
            case BUTTON_4:
                Menu_index=_DEF_MENU_MAIN;
                Menu_Action=0;
                break;



        }

    //ips200_clear_region(0,11*8,11*8+16,200);
        if(LastCursor!=Cursor){
            ips200_show_char(11*8,LastCursor*20,' ');

            LastCursor=Cursor;


        }

        ips200_show_char(11*8,Cursor*20,'<');

}

void Menu_AdjustGyro(){
    _FUNC_ADJUSTVALUE(Gyro_loop)

}

void Menu_AdjustAngle(){

    _FUNC_ADJUSTVALUE(Angle_loop)

}

void Menu_AdjustSpeed(){

    _FUNC_ADJUSTVALUE(Direction_loop)

}

void Menu_AdjustServo(){

    _FUNC_ADJUSTVALUE(ServoBalance_loop)

}

void Menu_ControlMotorAndServo(){


}

void Menu_GyroValue(){

    char TextGyro[30]={0};
    char TextEuler[30]={0};
    sprintf(TextGyro,"Gyro:%f",gyrof.x);
    sprintf(TextEuler,"Euler:%f",euler_deg.Pitch);
    ips200_show_string(0,0,TextGyro);
    ips200_show_string(0,20,TextEuler);
    //ips200_show_float(0,40,"GyroValue");
    //ips200_show_string(0,60,"Factors");
    //ips200_show_string(0,80,"BLT");
    //ips200_show_string(0,100,"Photo");

    switch(Menu_Action){
                    case BUTTON_4:
                        //ImgShowFlag=0;
                        Menu_index=_DEF_MENU_MAIN;
                        //ips200_clear();
                        break;
        }

}

void Menu_FactorSet(){
    static int Cursor=0;
    static int LastCursor=0;
    static int SetFlag=0;
    static float* Values[4]={&Vol.Normal,&Vol.Bend,&Vol.Island,&Vol.Cross};
    switch(Menu_Action){
        case BUTTON_1:
            if(SetFlag){
                *Values[Cursor-1]+=0.1;
            }else{
                Cursor--;
                Cursor=_FUNC_LIMITCURSOR(0,4,Cursor);
            }
            break;
        case BUTTON_2:
            if(Cursor==0){
                Img_RingFlag=!Img_RingFlag;
            }else{
                SetFlag=!SetFlag;
            }
            break;
        case BUTTON_3:
            if(SetFlag){
                *Values[Cursor-1]-=0.1;
            }else{
                Cursor++;
                Cursor=_FUNC_LIMITCURSOR(0,4,Cursor);
            }
            break;
        case BUTTON_4:
            Menu_index=_DEF_MENU_MAIN;
        break;
   }
}

void Menu_BLTRecv(){



}

void Menu_Photo(){

    switch(Menu_Action){
                case BUTTON_4:
                    ImgShowFlag=0;
                    Menu_index=_DEF_MENU_MAIN;
                    Motor.brake=1;
                                   Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag=0;
                    ips200_clear();
                    break;
    }

}

void Menu_AdjustForward(){

    _FUNC_ADJUSTVALUE(Forward_loop)

}

void Menu_AdjustDirection(){

    _FUNC_ADJUSTVALUE(Direction_loop)

}

void MenuRefresh_Main(){

    ips200_show_string(0,0,"ValueAdjust");
    ips200_show_string(0,20,"Controller");
    ips200_show_string(0,40,"GyroValue");
    ips200_show_string(0,60,"Factors");
    ips200_show_string(0,80,"BLT");
    ips200_show_string(0,100,"Photo");

}

void MenuRefresh_ValueAdjust(){


    ips200_show_string(0,0,"Gyro");
    ips200_show_string(0,20,"Angle");
    ips200_show_string(0,40,"Speed");
    ips200_show_string(0,60,"Servo");
    ips200_show_string(0,80,"Forward");
    ips200_show_string(0,100,"Direction");


}

void MenuRefresh_AdjustGyro(){

    ips200_show_string(0,0,"AdjustGyro");
    ips200_show_string(0,20,"KP:");
    ips200_show_string(0,40,"KI");
    ips200_show_string(0,60,"KD");
    ips200_show_string(0,80,"Step");

}

void MenuRefresh_AdjustAngle(){

    ips200_show_string(0,0,"AdjustAngle");
        ips200_show_string(0,20,"KP:");
        ips200_show_string(0,40,"KI");
        ips200_show_string(0,60,"KD");
        ips200_show_string(0,80,"Step");

}

void MenuRefresh_AdjustSpeed(){

    ips200_show_string(0,0,"AdjustDirection");
        ips200_show_string(0,20,"KP:");
        ips200_show_string(0,40,"KI");
        ips200_show_string(0,60,"KD");
        ips200_show_string(0,80,"Step");

}

void MenuRefresh_AdjustServo(){
    ips200_show_string(0,0,"AdjustServo");
        ips200_show_string(0,20,"KP:");
        ips200_show_string(0,40,"KI");
        ips200_show_string(0,60,"KD");
        ips200_show_string(0,80,"Step");


}

void MenuRefresh_ControlMotorAndServo(){


}

void MenuRefresh_GyroValue(){


}

void MenuRefresh_FactorSet(){
    ips200_show_string(0,0,"IslandOK?");
    ips200_show_string(0,20,"Normal");
    ips200_show_string(0,40,"Bend");
    ips200_show_string(0,60,"Island");
    ips200_show_string(0,80,"Cross");

}

void MenuRefresh_BLTRecv(){}

void MenuRefresh_Photo(){


    ImgShowFlag=1;


}

void MenuRefresh_AdjustForward(){

    ips200_show_string(0,0,"AdjustForward");
            ips200_show_string(0,20,"KP:");
            ips200_show_string(0,40,"KI");
            ips200_show_string(0,60,"KD");
            ips200_show_string(0,80,"Step");
}
void MenuRefresh_Direction(){

    ips200_show_string(0,0,"AdjustDirection");
            ips200_show_string(0,20,"KP:");
            ips200_show_string(0,40,"KI");
            ips200_show_string(0,60,"KD");
            ips200_show_string(0,80,"Step");
}

