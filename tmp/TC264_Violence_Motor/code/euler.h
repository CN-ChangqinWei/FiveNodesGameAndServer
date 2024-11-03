/*
 * Complement.h
 *
 *  Created on: 2023锟斤拷12锟斤拷26锟斤拷
 *      Author: Roronoa zoro
 */

#ifndef CODE_EULER_H_
#define CODE_EULER_H_
#include "zf_common_headfile.h"
#include "filter.h"
//#define _DEF_IMU963 0

#ifndef _DEF_IMU963



#define gyro_x              imu660ra_gyro_x
#define gyro_y              imu660ra_gyro_y
#define gyro_z              imu660ra_gyro_z
#define acc_x               imu660ra_acc_x
#define acc_y               imu660ra_acc_y
#define acc_z               imu660ra_acc_z
#define IMU_Init(void)          imu660ra_init()
#define IMU_Get_gyro(void)      imu660ra_get_gyro()
#define IMU_Get_acc(void)       imu660ra_get_acc()
#define gyro_transition(x)   imu660ra_gyro_transition(x)
#define acc_transition(x)    imu660ra_acc_transition(x)
#else
#define gyro_x              imu963ra_gyro_x
#define gyro_y              imu963ra_gyro_y
#define gyro_z              imu963ra_gyro_z
#define acc_x               imu963ra_acc_x
#define acc_y               imu963ra_acc_y
#define acc_z               imu963ra_acc_z
#define IMU_Init(void)          imu963ra_init()
#define IMU_Get_gyro(void)      imu963ra_get_gyro()
#define IMU_Get_acc(void)       imu963ra_get_acc()
#define gyro_transition(x)   imu963ra_gyro_transition(x)
#define acc_transition(x)    imu963ra_acc_transition(x)
#endif
typedef struct
{
    float x;
    float y;
    float z;
}Descartes_vector;

typedef struct
{
    Descartes_vector offset;
    Descartes_vector Orign;
    Descartes_vector temp;
}Mag_Adjust;

typedef struct
{
    float Pitch;
    float Roll;
    float Yaw;
}attitude;
extern biquadFilter_t gyroFilterLPF[3],accFilterLPF[3];//锟斤拷锟阶碉拷通锟剿诧拷锟斤拷
extern float Angle_x_temp;         //锟缴硷拷锟劫度硷拷锟斤拷锟絰锟斤拷斜锟角讹拷
extern float Angle_y_temp;         //锟缴硷拷锟劫度硷拷锟斤拷锟統锟斤拷斜锟角讹拷
extern Axis3f gyrof,accf,gyrof_bias,accf_bias;
extern attitude euler_deg;         //欧锟斤拷锟角角讹拷
extern attitude euler_rad;         //欧锟斤拷锟角伙拷锟斤拷
extern Mag_Adjust Mag;
extern float mag_angle;        //锟截磁硷拷校准锟斤拷慕嵌锟�
extern float Pitch_temp,gyro_z_temp;
extern float gyro_z_bias,gyro_x_bias,gyro_y_bias;
extern float Q_angle,Q_gyro,R_angle,Q_bias; // 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷协锟斤拷锟斤拷
extern float AHRS_Kp,AHRS_Ki;
void READ_MPU963(void);
void Get_euler(void);
void IMU_get_euler(void);


#endif /* CODE_EULER_H_ */
