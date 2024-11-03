/*
 * pid.c
 *
 *  Created on: 2024年1月2日
 *      Author: Roronoa zoro
 */
#include "pid.h"
#include "Img_Error.h"
#define No_Limit    10000
#define Zero_Angle  0.05
#define Car_Num     1
#define _FUNC_LIMIT(value,min,max) (value<min?min:(value>max?max:value))
int ForwardFlag=0;
float openPWM=2500;
float Angle_IN;
float Angle_Max=3;
float Angle_Mid=1.5;
float Angle_Gain=0.0027;
float Angle_Field=15;
int FastSpeed=2800;
int FastFlag=0;
Relay_Type Relay_Angle;
uint8 Obstacle=0;
uint16 Obstacle_PWM=0,Obstacle_Dis=600;
float bounds[6]={20,25,30,35,40,45};
float ys[7]={0,0.5,1,1.5,2,2.5,3.0};
float Forward_Control,Forward_Kp;
PID_TYPE    PIDS[3];
///float Angle_Delta=0;
//定义PID结构体
PID_TYPE    Gyro_loop,Angle_loop,Speed_loop,Zgyro_loop,Forward_loop,ServoBalance_loop,Direction_loop,AngleTarget_loop;
Voltages Vol={
       .Bend=_DEF_VOL_BEND,
       .Cross=_DEF_VOL_CROSS,
       .Normal=_DEF_VOL_NORMAL,
       .Island=_DEF_VOL_ISLAND
};
//角度环期望
float   Angle_Target,Speed_Target;

float LIMT(float Number,float Min,float Max)
{
    if(Number<Min)
        Number = Min;
    else if(Number>Max)
        Number = Max;
    return Number;
}
/*****************************************************************************
* 函  数：void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
* 功  能：位置式PID算法
* 参  数：PID: 算法P I D参数的结构体
*       target: 目标值(输入值)
*       measure: 测量值
*       Pwm=Kp*e(k)+Ki*∑ e(k)+Kd[e(k)-e(k-1)]
* 返回值：无
* 备  注:
*****************************************************************************/


void PID_Param_ZeroOne(PID_TYPE*PID){
    PID->D=0;
    PID->P=0;
    PID->I=0;
}



void PID_Param_Zero(int mode){

    switch(mode){
            case _DEF_ZERO_ALL:
                PID_Param_ZeroOne(&Speed_loop);
                PID_Param_ZeroOne(&Angle_loop);
                PID_Param_ZeroOne(&Gyro_loop);
                break;
            case _DEF_DEFAULT_SPEED:
                PID_Param_ZeroOne(&Speed_loop);
                break;
            case _DEF_DEFAULT_ANGLE:
                PID_Param_ZeroOne(&Angle_loop);
                break;
            case   _DEF_DEFAULT_GYRO:
                PID_Param_ZeroOne(&Gyro_loop);
                break;
            case   _DEF_DEFAULT_SERVO:

                break;
            case   _DEF_DEFAULT_FORWARD:

                break;
        }
}


void PID_Param_Default(int mode){

    switch(mode){

        case _DEF_DEFAULT_ALL:
            _FUNC_DEFAULT_RING(SPEED,Speed)
            _FUNC_DEFAULT_RING(ANGLE,Angle)
            _FUNC_DEFAULT_RING(GYRO,Gyro)
            break;
        case _DEF_DEFAULT_SPEED:
            _FUNC_DEFAULT_RING(SPEED,Speed)
            break;
        case _DEF_DEFAULT_ANGLE:
            _FUNC_DEFAULT_RING(ANGLE,Angle)
            break;
        case   _DEF_DEFAULT_GYRO:
            _FUNC_DEFAULT_RING(GYRO,Gyro)
            break;
        case   _DEF_DEFAULT_SERVO:

            break;
        case   _DEF_DEFAULT_FORWARD:

            break;
    }


}


void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
{
    if(PID->out_flag==0)
    {
        PID->Error=0;PID->Integral=0;PID->Differ=0;
        PID->Pout=0;PID->Iout=0;PID->Dout=0;PID->OutPut=0;
        return;
    }
    PID->Error  = target - measure;                                 //误差
    PID->Differ = PID->Error - PID->PreError;                           //微分量

    PID->Pout = PID->P * PID->Error;                                    //比例控制
    PID->Iout += PID->I * PID->Error;              //积分控制
    PID->Dout = PID->D * PID->Differ;                                   //微分控制
if(PID->Iout > PID->Irang)                      //积分限幅
        PID->Iout = PID->Irang;
    if(PID->Iout < -PID->Irang)                     //积分限幅
        PID->Iout = -PID->Irang;
    PID->OutPut =PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
    PID->OutPut =LIMT(PID->OutPut,-PID->OutPut_Limit,PID->OutPut_Limit);


    PID->PreError = PID->Error;
}

void PID_Postion_Cal2(PID_TYPE*PID,float target,float measure)
{
    if(PID->out_flag==0)
    {
        PID->Error=0;PID->Integral=0;PID->Differ=0;
        PID->Pout=0;PID->Iout=0;PID->Dout=0;PID->OutPut=0;
        return;
    }
    PID->Error  = target - measure;                                 //误差
    PID->Differ = PID->Error - PID->PreError;                           //微分量

    PID->Pout = PID->P * PID->Error;                                    //比例控制
                  //积分控制
    PID->Dout = PID->D * PID->Differ;                                   //微分控制
    if(_FUNC_ABS(PID->Error)<PID->Ilimit)
        PID->Iout += PID->I * PID->Error;
if(PID->Iout > PID->Irang)                      //积分限幅
        PID->Iout = PID->Irang;
    if(PID->Iout < -PID->Irang)                     //积分限幅
        PID->Iout = -PID->Irang;
    PID->OutPut =PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
    PID->OutPut =LIMT(PID->OutPut,-PID->OutPut_Limit,PID->OutPut_Limit);


    PID->PreError = PID->Error;
}

/*****************************************************************************
* 函  数：void Pid_increment_Cal(PID_TYPE*PID,float target,float measure)
* 功  能：增量式PID
* 注  意：Pwm=Kp[e(k)-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
* 返回值：无
* 备  注:
*****************************************************************************/
void Pid_increment_Cal(PID_TYPE*PID,float target,float measure)
{
//    if(PID->out_flag==0)
//    {
//        PID->Error=0;PID->Integral=0;PID->Differ=0;
//        PID->Pout=0;PID->Iout=0;PID->Dout=0;PID->OutPut=0;
//        return;
//    }
    PID->Error  = target - measure;                                 //误差
    PID->Pout = PID->P * (PID->Error-PID->PreError);                        //比例控制
    PID->Iout = _FUNC_LIMIT(PID->I * PID->Error,-PID->Irang,PID->Irang);                                        //积分控制
    PID->Dout = PID->D * (PID->Error-2*PID->PreError+PID->PrePreError);     //微分控制
    PID->OutPut =  PID->LastOutPut+PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
    PID->OutPut =_FUNC_LIMIT(PID->OutPut,-PID->OutPut_Limit,PID->OutPut_Limit);
//    if(PID->Integral > PID->Irang)     //积分限幅
//        PID->Integral = PID->Irang;
//    if(PID->Integral < -PID->Irang)    //积分限幅
//        PID->Integral = -PID->Irang;

    PID->PrePreError=PID->PreError; //记忆e(k-2)
    PID->PreError=PID->Error;       //记忆e(k-1)
    PID->LastOutPut=PID->OutPut;
}
void PID_Param_Init(void)
{


    Relay_Angle.Error=0;
    Relay_Angle.dx=10;
    Relay_Angle.flag=0;
    Relay_Angle.target=&Angle_Target;
    Relay_Angle.bound=bounds;
    Relay_Angle.y=ys;

    Relay_Angle.nofbound=6;
    Relay_Angle.Zero_Value=_DEF_DEFAULT_TARGETANGLE;

//    Gyro_loop.P=0;
//    Gyro_loop.I=0;
//    Gyro_loop.D=0;
    Gyro_loop.P=_DEF_DEFAULT_GYRO_KP;
    Gyro_loop.I=_DEF_DEFAULT_GYRO_KI;
    Gyro_loop.D=_DEF_DEFAULT_GYRO_KD;
    Gyro_loop.Ilimit=2000;          //积分分离限制为角速度量程
    Gyro_loop.OutPut_Limit=8000;//角速度环限幅
    Gyro_loop.Irang=6000;
    Gyro_loop.out_flag=0;

    //角度环参数初始化
    Angle_Target=_DEF_DEFAULT_TARGETANGLE;

    Angle_loop.P=_DEF_DEFAULT_ANGLE_KP;
    Angle_loop.I=_DEF_DEFAULT_ANGLE_KI;
    Angle_loop.D=_DEF_DEFAULT_ANGLE_KD;
    Angle_loop.Ilimit=180;          //角度积分限幅
    Angle_loop.OutPut_Limit=2000;   //角度环输出的限幅是角速度量程
    Angle_loop.Irang=2000;
    Angle_loop.out_flag=0;

//    Speed_loop.P=-0.083534;
//    Speed_loop.I=-0.00002992;
//    Speed_loop.D=0.102;
//    Speed_loop.Ilimit=5000;          //角度积分限幅
//    Speed_loop.OutPut_Limit=10000;   //角度环输出的限幅是角速度量程
//    Speed_loop.Irang=12000;
//    Speed_loop.out_flag=1;
//
    Zgyro_loop.P=_DEF_DEFAULT_ZGYRO_KP;
    Zgyro_loop.I=_DEF_DEFAULT_ZGYRO_KI;
    Zgyro_loop.D=_DEF_DEFAULT_ZGYRO_KD;
    Zgyro_loop.Ilimit=2000;          //角度积分限幅
    Zgyro_loop.OutPut_Limit=30;   //角度环输出的限幅是角速度量程
    Zgyro_loop.Irang=8000;
    Zgyro_loop.out_flag=0;

    AngleTarget_loop.P=_DEF_DEFAULT_ANGLETARGET_KP;
    AngleTarget_loop.I=_DEF_DEFAULT_ANGLETARGET_KI;
    AngleTarget_loop.D=_DEF_DEFAULT_ANGLETARGET_KD;

    Forward_loop.P=_DEF_DEFAULT_FORWARD_KP;
    Forward_loop.I=_DEF_DEFAULT_FORWARD_KI;
    Forward_loop.D=_DEF_DEFAULT_FORWARD_KD;
    Forward_loop.Ilimit=200;          //角度积分限幅
    Forward_loop.OutPut_Limit=5000;   //角度环输出的限幅是角速度量程
    Forward_loop.Irang=5000;
    Forward_loop.out_flag=0;
    Forward_loop.LastOutPut=0;

    ServoBalance_loop.P=_DEF_DEFAULT_SERVO_KP;
        ServoBalance_loop.I=_DEF_DEFAULT_SERVO_KI;
        ServoBalance_loop.D=_DEF_DEFAULT_SERVO_KD;
        ServoBalance_loop.Ilimit=0;          //角度积分限幅
        ServoBalance_loop.OutPut_Limit=150;   //角度环输出的限幅是角速度量程
        ServoBalance_loop.Irang=0;
        ServoBalance_loop.out_flag=0;
        ServoBalance_loop.LastOutPut=0;

    Direction_loop.P=_DEF_DEFAULT_DIRECTION_KP;
    Direction_loop.I=_DEF_DEFAULT_DIRECTION_KI;
    Direction_loop.D=_DEF_DEFAULT_DIRECTION_KD;
    Direction_loop.Ilimit=30;          //角度积分限幅
    Direction_loop.OutPut_Limit=30;   //角度环输出的限幅是角速度量程
    Direction_loop.Irang=7;
    Direction_loop.out_flag=0;
    PIDS[0]=PIDS[1]=PIDS[2]=Direction_loop;
    PIDS[1].P=_DEF_DEFAULT_DIRECTION_KP_MID;
    PIDS[1].I=_DEF_DEFAULT_DIRECTION_KI_MID;
    PIDS[1].D=_DEF_DEFAULT_DIRECTION_KD_MID;
    PIDS[0].P=_DEF_DEFAULT_DIRECTION_KP_SLOW;
    PIDS[0].I=_DEF_DEFAULT_DIRECTION_KI_SLOW;
    PIDS[0].D=_DEF_DEFAULT_DIRECTION_KD_SLOW;

}
void Obstacle_process(void)
{
    static uint8 state=0;
    static uint16 dis[6]={8190,8190,8190,8190,8190,8190};
    float sum=0;
    dl1a_get_distance();
    if(dl1a_finsh_flag == 1)
    {
      dl1a_finsh_flag = 0;
      for(uint8 i=0;i<5;i++)
      {
          dis[i]=dis[i+1];
          sum+=dis[i+1];
      }
      dis[5]=dl1a_distance_mm;
      sum+=dis[5];
      distance=sum/6;
//      if(Motor.brake==1)
//      {
//          state=0;
//      }
      switch(state)
      {
          case 0: Obstacle_PWM=0;Lcd_index_cnt=0;
                  if(distance<=Obstacle_Dis)
                  {
                      Obstacle=1;
                      state=1;
                      Obstacle_PWM=3000;
                  }
              break;
          case 1:

                  if(euler_deg.Roll>8)
                  {
                      state=2;
                      Obstacle_PWM=4000;
                  }
              break;
          case 2:
                  if(euler_deg.Roll<-10)
                  {
                      state=3;
                      Obstacle_PWM=2000;
                  }
              break;
          case 3: if(distance>Obstacle_Dis&&euler_deg.Roll>=-10)
                  {
                      state=4;
                      Obstacle_PWM=0;
                      Obstacle=0;
//                      Lcd_index_cnt=1;
                  }
              break;
          case 4: //if(Lcd_index_cnt==0)state=0;
              break;
      }

    }
}
void Gyro_2ms(void)
{

    //角速度环PID计算
    PID_Postion_Cal(&Gyro_loop,Angle_loop.OutPut,gyrof.y);

        if(ForwardFlag){

            if(euler_deg.Roll>6){
                Motor.duty_L=-Gyro_loop.OutPut-4000;
                Motor.duty_R=Gyro_loop.OutPut-4000;
            }else if(euler_deg.Roll<-10){
                Motor.duty_L=-Gyro_loop.OutPut-2000;
                Motor.duty_R=Gyro_loop.OutPut-2000;
            }
            else{
                Motor.duty_L=-Gyro_loop.OutPut-openPWM;
                Motor.duty_R=Gyro_loop.OutPut-openPWM;
            }
        }else{
            Motor.duty_L=-Gyro_loop.OutPut;
            Motor.duty_R=Gyro_loop.OutPut;
        }


    Set_PWM(&Motor);
}

void Angle_10ms(void)
{
    if(euler_deg.Pitch>38||euler_deg.Pitch<-38)
    {
        Motor.brake=1;
        ServoBalance_loop.out_flag=Forward_loop.out_flag=Angle_loop.out_flag=Direction_loop.out_flag=Gyro_loop.out_flag=0;
        nCross=0;
    }
    Angle_IN=Angle_Target-Direction_loop.OutPut+Zgyro_loop.OutPut;
   // Angle_IN=_FUNC_LIMIT(Angle_IN,-14,14);
    PID_Postion_Cal(&Angle_loop,Angle_IN,euler_deg.Pitch);

}
#define _FUNC_SIG(value) (value<0?(-1):1)

void Relay(Relay_Type* relay){
    int up, down;
        if ((relay->flag) > 0) {
            if (_FUNC_ABS(relay->flag) < relay->nofbound)
                up = (int)(_FUNC_SIG(relay->flag) * relay->bound[_FUNC_ABS(relay->flag)]);
            else up = 2147483647;
            down = relay->bound[_FUNC_ABS(relay->flag) - 1] - relay->dx;
            if (relay->Error > up) {
                relay->flag++;
                *relay->target = relay->Zero_Value + relay->y[relay->flag];
            }
            else if (relay->Error < down) {

                relay->flag--;
                *relay->target = relay->Zero_Value + relay->y[relay->flag];
            }
        }
        else if ((relay->flag) < 0) {

            if (_FUNC_ABS(relay->flag) < relay->nofbound)
                down = (int)(_FUNC_SIG(relay->flag) * relay->bound[_FUNC_ABS(relay->flag)]);
            else down = -214748364;
            up = -relay->bound[_FUNC_ABS(relay->flag) - 1] + relay->dx;
            if (relay->Error > up) {
                relay->flag++;
                *relay->target = relay->Zero_Value - relay->y[-relay->flag];
            }
            else if (relay->Error < down) {

                relay->flag--;
                *relay->target = relay->Zero_Value - relay->y[-relay->flag];
            }
        }
        else {
            up = relay->bound[0];
            down = -relay->bound[0];
            if (relay->Error > up) {
                relay->flag++;

            }
            else if (relay->Error < down) {
                relay->flag--;

            }
            *relay->target = relay->Zero_Value + _FUNC_SIG(relay->flag) * relay->y[_FUNC_ABS(relay->flag)];
        }

}
float Dynamic_Zero=1.2,Dynamic_delt=0;
void Direction_10ms(void){
    //Relay_Angle.Error=Error;
    //Relay(&Relay_Angle);
//   if(_FUNC_ABS(Error)<20) openPWM=2400;
//    else{
//        openPWM=2400-(_FUNC_ABS(openPWM)-20)*5;
//
//    }
    Angle_Target=_DEF_DEFAULT_TARGETANGLE+Error*(Dynamic_Zero/57);
    if(_FUNC_ABS(Error)>Angle_Field)
    {
        Motor_Voltage=Vol.Bend;
        FastFlag=0;
    }else if(Img_S<10&&Keys[2]&&StartTime>20){
        //Motor_Voltage=Vol.Normal+1;
        FastFlag=1;
        openPWM=FastSpeed;
    }
    else
    {
        Motor_Voltage=Vol.Normal;
        FastFlag=0;
    }
    //Motor_Voltage=1;
    PID_Postion_Cal(&Direction_loop,0,Error);


}

void Speed_50ms(void)
{

    Speed_loop.OutPut/=1000;
//    PID_Postion_Cal(&Forward_loop,Speed_Target,Encoder_Sum);
}
void GyroZ_50ms(void)
{

    PID_Postion_Cal(&Zgyro_loop,0,gyrof.z);
    //Zgyro_loop.OutPut/=1000;
}
void Servo_50ms(void){
    PID_Postion_Cal(&ServoBalance_loop,0,Error);
}

void Forward_50ms(){



}

void Clear_Pid(void)
{
    Gyro_loop.P=0;
    Gyro_loop.I=0;
    Gyro_loop.D=0;

    Angle_loop.P=0;
    Angle_loop.I=0;
    Angle_loop.D=0;

    Zgyro_loop.P=0;
    Zgyro_loop.I=0;
    Zgyro_loop.D=0;
}
void Set_Pid(void)
{
    Gyro_loop.P=9.863;
    Gyro_loop.I=0.4141;
    Gyro_loop.D=28.58;

    Angle_loop.P=25.098;
    Angle_loop.I=0;
    Angle_loop.D=-23.176;

    Speed_loop.P=-0.083534;
    Speed_loop.I=-0.00002992;
    Speed_loop.D=0.102;
}
