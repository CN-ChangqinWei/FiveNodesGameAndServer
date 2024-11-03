/*
 * key.c
 *
 *  Created on: 2023年12月29日
 *      Author: Roronoa zoro
 */
#include "key.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     按键扫描，可判断单击长按（用户定义）
// 参数说明     无
// 返回参数     void
// 使用示例     放在定时器中断中调用，10ms执行一次
// 备注信息     外部调用Trig，Long_flag来确认按键状态
//-------------------------------------------------------------------------------------------------------------------
Button Key;
uint8 Keys[4]={0};
uint8 LastKeys[4]={0};
int StartFlag=0;
Voltages MotorVols[3];
void Key_read(Button* key)
{
    static uint16 key_time[KEY_NUMBER]={0};
    uint8 i=0;
    uint8 Readdata=0;
    if(gpio_get_level(Key_ON)==0)Readdata|=0x01;//KEY_1
    if(gpio_get_level(Key_Down)==0)Readdata|=0x02;//KEY_2
    if(gpio_get_level(Key_Left)==0)Readdata|=0x04;//KEY_3
    if(gpio_get_level(Key_Right)==0)Readdata|=0x08;//KEY_4
    if(gpio_get_level(Key_Center)==0)Readdata|=0x10;//KEY_4
    Keys[0]=gpio_get_level(_DEF_KEYS_1);
    Keys[1]=gpio_get_level(_DEF_KEYS_2);
    Keys[2]=gpio_get_level(_DEF_KEYS_3);
    Keys[3]=gpio_get_level(_DEF_KEYS_4);
    key->Trig=Readdata&(Readdata^key->Cont);
    key->Cont=Readdata;
    //长按检测
    switch(key-> Cont^0x00)
    {
        //按键按下后开始计时
        case 0x01:key_time[0]++;break;
        case 0x02:key_time[1]++;break;
        case 0x04:key_time[2]++;break;
        case 0x08:key_time[3]++;break;
        default:break;
    }

}
#define _FUNC_CURLIM(cur,min,max) (cur>max?min:(cur<min?max:cur))
void Key_process(void)
{

    static flag=0;
    static uint8 Keys_Last[4]={0};
    static uint32* pKeys_Last=(uint32*)Keys_Last;
    static uint32* pKeys=(uint32*)Keys;
    switch(Key.Trig)
    {
        case BUTTON_1:
            Img_ExposureTime+=10;
            Img_ExposureTime=_FUNC_CURLIM(Img_ExposureTime,0,200);
            mt9v03x_set_exposure_time(Img_ExposureTime);
            break;
        case BUTTON_2:
            if(flag==0){
                flag=1;
                Motor.brake=0;
                Gyro_loop.out_flag=Angle_loop.out_flag=ServoBalance_loop.out_flag=Zgyro_loop.out_flag=1;
                Gyro_loop.Iout=Angle_loop.Iout=Direction_loop.Iout=0;
                ForwardFlag=0;
                RoadStat=0;
                StartTime=0;
            }else if(flag==1){
                flag=0;
                Direction_loop.out_flag=1;
                ForwardFlag=1;
                RoadStat=0;
                StartTime=0;
            }
        break;
        case BUTTON_3:
            Img_ExposureTime-=10;
            Img_ExposureTime=_FUNC_CURLIM(Img_ExposureTime,0,200);
            mt9v03x_set_exposure_time(Img_ExposureTime);
            break;
        case BUTTON_4:ImgShowFlag=!ImgShowFlag;break;
    }
    if(*pKeys^*pKeys_Last&&Motor.brake==1&&ImgShowFlag==0){
        for(int i=0;i<3;i++)
            if(!Keys[i]){
                Vol=MotorVols[i];
                Direction_loop=PIDS[i];
                break;
            }
        Img_RingFlag=Keys[3];
        ips200_show_float(0,0,Vol.Bend,3,1);
        ips200_show_float(40,0,Vol.Cross,3,1);
        ips200_show_float(80,0,Vol.Island,3,1);
        ips200_show_float(120,0,Vol.Normal,3,1);
        ips200_show_int(0,20,Keys[0],3);
        ips200_show_int(40,20,Keys[1],3);
        ips200_show_int(80,20,Keys[2],3);
        ips200_show_int(120,20,Keys[3],3);

    }
    switch(Key.Long)
    {
        case BUTTON_1:Motor.brake=1;break;
        case BUTTON_2:Motor.brake=0;break;
        case BUTTON_3:break;
        case BUTTON_4:break;
    }

    *pKeys_Last=*pKeys;
}
