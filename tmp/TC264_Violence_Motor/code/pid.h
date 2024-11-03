/*
 * pid.h
 *
 *  Created on: 2024年1月2日
 *      Author: Roronoa zoro
 */

#ifndef CODE_PID_H_
#define CODE_PID_H_
#include "zf_common_headfile.h"
#include "Img.h"
#include "Task.h"
typedef struct Voltage{
        float Normal;
        float Bend;
        float Cross;
        float Island;
}Voltages;
extern Voltages MotorVols[3];
typedef struct PID
{
  float P;         //参数
  float I;
  float D;
  float Error;     //比例项e(k)
  float Integral;  //积分项
  float Differ;    //微分项
  float PreError;   //e(k-1)
  float PrePreError;//e(k-2)
  float Ilimit;
  float Irang;
  double Pout;
  double Iout;
  double Dout;
  double OutPut;
  double LastOutPut;
  uint16 OutPut_Limit;
  uint8_t Ilimit_flag;    //积分分离
  uint8_t out_flag;

}PID_TYPE;
typedef struct RELAY{
        float Error;
        float Zero_Value;
        float dx;
        float *target;
        float *bound;
        float *y;
        int nofbound;
        int flag;
}Relay_Type;
extern PID_TYPE    Gyro_loop,Angle_loop,Speed_loop,Zgyro_loop,Forward_loop,ServoBalance_loop,Direction_loop,AngleTarget_loop;
extern PID_TYPE    PIDS[3];
extern float   Angle_Target,Speed_Target;
extern uint8 Obstacle;
extern uint16 Obstacle_PWM;
extern int ForwardFlag;
extern float openPWM;
extern float Angle_IN;
extern float Angle_Field;
extern float Angle_Gain,Dynamic_Zero,Forward_Control,Forward_Kp,Dynamic_Error,Dynamic_delt;
extern Voltages Vol;
extern int FastSpeed;
extern int FastFlag;
//Voltages
//FAST
#define _DEF_VOL_NORMAL             (3.2)

#define _DEF_VOL_BEND               (3.2)

#define _DEF_VOL_CROSS              (3.2)

#define _DEF_VOL_ISLAND             (3.2)
//MID
#define _DEF_VOL_NORMAL_MID             (2.1)

#define _DEF_VOL_BEND_MID               (2.1)

#define _DEF_VOL_CROSS_MID              (2.1)

#define _DEF_VOL_ISLAND_MID             (2.1)
//SLOW
#define _DEF_VOL_NORMAL_SLOW             (2.1)

#define _DEF_VOL_BEND_SLOW               (2.1)

#define _DEF_VOL_CROSS_SLOW              (2.1)

#define _DEF_VOL_ISLAND_SLOW             (2.1)

//pid

#define _DEF_ZERO_ALL             (0)

#define _DEF_ZERO_SPEED           (1)

#define _DEF_ZERO_ANGLE           (2)

#define _DEF_ZERO_GYRO            (3)

#define _DEF_ZERO_SERVO           (4)

#define _DEF_ZERO_FORWARD         (5)

#define _DEF_DEFAULT_SPEED_KP     (-0.083534)

#define _DEF_DEFAULT_SPEED_KI     (-0.00002992)

#define _DEF_DEFAULT_SPEED_KD     (0.102)

#define _DEF_DEFAULT_ANGLE_KP     (55)//角度环Kp

#define _DEF_DEFAULT_ANGLE_KI     (0)

#define _DEF_DEFAULT_ANGLE_KD     (-20)

#define _DEF_DEFAULT_GYRO_KP     (14.5)

#define _DEF_DEFAULT_GYRO_KI     (0.2)

#define _DEF_DEFAULT_GYRO_KD     (100)

#define _DEF_DEFAULT_SERVO_KP     (1.585)


#define _DEF_DEFAULT_SERVO_KI     (0)

#define _DEF_DEFAULT_SERVO_KD     (0)

#define _DEF_DEFAULT_FORWARD_KP     (-6.8)

#define _DEF_DEFAULT_FORWARD_KI     (-0.7)

#define _DEF_DEFAULT_FORWARD_KD     (0)
//FAST
#define _DEF_DEFAULT_DIRECTION_KP   (0.25)

#define _DEF_DEFAULT_DIRECTION_KI   (0)

#define _DEF_DEFAULT_DIRECTION_KD   (-0.15)

//MID
#define _DEF_DEFAULT_DIRECTION_KP_MID   (0.22)

#define _DEF_DEFAULT_DIRECTION_KI_MID   (0)

#define _DEF_DEFAULT_DIRECTION_KD_MID   (-0.06)
//SLOW
#define _DEF_DEFAULT_DIRECTION_KP_SLOW   (0.2)

#define _DEF_DEFAULT_DIRECTION_KI_SLOW   (0)

#define _DEF_DEFAULT_DIRECTION_KD_SLOW   (-0.05)





#define _DEF_DEFAULT_ANGLETARGET_KP   (0.05)

#define _DEF_DEFAULT_ANGLETARGET_KI   (0)

#define _DEF_DEFAULT_ANGLETARGET_KD   (0)


#define _DEF_DEFAULT_ZGYRO_KP   (0)

#define _DEF_DEFAULT_ZGYRO_KI   (0)

#define _DEF_DEFAULT_ZGYRO_KD   (0)


#define _DEF_DEFAULT_TARGETANGLE    (1.0)

//------------------ZeroCMD--------------
#define _DEF_CMD_ZERO             (0xAAFF010D)

#define _DEF_ZERO_ALL             (0)

#define _DEF_ZERO_SPEED           (1)

#define _DEF_ZERO_ANGLE           (2)

#define _DEF_ZERO_GYRO            (3)

#define _DEF_ZERO_SERVO           (4)

#define _DEF_ZERO_FORWARD         (5)


//------------------DefaultCMD
#define _DEF_CMD_DEFAULT          (0xAAFF010E)

#define _DEF_DEFAULT_ALL             (0)

#define _DEF_DEFAULT_SPEED           (1)

#define _DEF_DEFAULT_ANGLE           (2)

#define _DEF_DEFAULT_GYRO            (3)

#define _DEF_DEFAULT_SERVO           (4)

#define _DEF_DEFAULT_FORWARD         (5)

#define _FUNC_DEFAULT_RING(RING,ring) {\
        ring##_loop.P=_DEF_DEFAULT_##RING##_KP;\
        ring##_loop.I=_DEF_DEFAULT_##RING##_KI;\
        ring##_loop.D=_DEF_DEFAULT_##RING##_KD;\
}
void PID_Param_Zero(int mode);
void Servo_50ms(void);
void PID_Param_Default(int mode);
void PID_Param_Init(void);
void Gyro_2ms(void);
void Angle_10ms(void);
void Direction_10ms(void);
void Speed_50ms(void);
void GyroZ_50ms(void);
void Clear_Pid(void);
void Set_Pid(void);
void Forward_50ms();
void Obstacle_process(void);
#endif /* CODE_PID_H_ */
