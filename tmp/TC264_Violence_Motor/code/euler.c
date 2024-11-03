/*
 * Complement.c
 *
 *  Created on: 2023骞�12鏈�26鏃�
 *      Author: Roronoa zoro
 */
#include "euler.h"
float mag_angle;        //鍦扮璁℃牎鍑嗗悗鐨勮搴�
float mag_ratio =0.4;      //鍦扮璁℃瘮渚�
float gyro_ratio =0.6;    //闄�铻轰华姣斾緥
float dt =0.002;            //閲囨牱鍛ㄦ湡
attitude euler_deg;         //娆ф媺瑙掕搴�
attitude euler_rad;         //娆ф媺瑙掑姬搴�

Descartes_vector gyro,acc,mag,mag_last;

biquadFilter_t gyroFilterLPF[XYZ_AXIS_COUNT];//浜岄樁浣庨�氭护娉㈠櫒
biquadFilter_t accFilterLPF[XYZ_AXIS_COUNT];//浜岄樁浣庨�氭护娉㈠櫒
Axis3f gyrof,accf,gyrof_bias,accf_bias;           //杞崲鍗曚綅涓郝�/s鐨勬暟鎹�

void lowpass(float * measure,float weight,uint8 p)
{
    static float measure_temp[3]={0};
    *measure=measure_temp[p]*weight+(*measure)*(1-weight);
    measure_temp[p]=*measure;
}

float Angle_x_temp;         //鐢卞姞閫熷害璁＄畻鐨剎鍊炬枩瑙掑害
float Angle_y_temp;         //鐢卞姞閫熷害璁＄畻鐨剏鍊炬枩瑙掑害
float bias;
float angle_dot_x,angle_dot_y;
float gyro_z_bias,gyro_x_bias,gyro_y_bias;
float Pitch_temp,gyro_z_temp;

void READ_MPU963(void)
{

    IMU_Get_acc();
    IMU_Get_gyro();
    gyrof.x=gyro_transition(gyro_x)-gyrof_bias.x;
    gyrof.y=gyro_transition(gyro_y)-gyrof_bias.y;
    gyrof.z=gyro_transition(gyro_z)-gyrof_bias.z;
    accf.x=acc_transition(acc_x);
    accf.y=acc_transition(acc_y);
    accf.z=acc_transition(acc_z);
    for (int axis = 0; axis < 3; axis++)
    {
        gyrof.axis[axis] = biquadFilterApply(&gyroFilterLPF[axis], gyrof.axis[axis]);
    }
    for (int axis = 0; axis < 3; axis++)
    {
        accf.axis[axis] = biquadFilterApply(&accFilterLPF[axis], accf.axis[axis]);
    }
    if(gyrof.z<0.06&&gyrof.z>-0.06)gyrof.z=0;

}


float Q_angle=0.001; // 杩囩▼鍣０鐨勫崗鏂瑰樊
float Q_gyro=0.003; //0.003 杩囩▼鍣０鐨勫崗鏂瑰樊 杩囩▼鍣０鐨勫崗鏂瑰樊涓轰竴涓竴琛屼袱鍒楃煩闃�
float R_angle=0.5,Q_bias;      // 娴嬮噺鍣０鐨勫崗鏂瑰樊 鏃㈡祴閲忓亸宸�

float Kalman_Filter_x(float Accel,float Gyro)
{
    static float angle;
    char  C_0 = 1;
    static  float Angle_err;
    static float PCt_0, PCt_1, E;
    static float K_0, K_1, t_0, t_1;
    static float Pdot[4] ={0,0,0,0};
    static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
    angle+=(Gyro - Q_bias) * dt; //鍏堥獙浼拌
    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-鍏堥獙浼拌璇樊鍗忔柟宸殑寰垎

    Pdot[1]=-PP[1][1];
    Pdot[2]=-PP[1][1];
    Pdot[3]=Q_gyro;
    PP[0][0] += Pdot[0] * dt;   // Pk-鍏堥獙浼拌璇樊鍗忔柟宸井鍒嗙殑绉垎
    PP[0][1] += Pdot[1] * dt;   // =鍏堥獙浼拌璇樊鍗忔柟宸�
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - angle;  //zk-鍏堥獙浼拌

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //鍚庨獙浼拌璇樊鍗忔柟宸�
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    angle   += K_0 * Angle_err;  //鍚庨獙浼拌
    Q_bias  += K_1 * Angle_err;  //鍚庨獙浼拌
    angle_dot_x   = Gyro - Q_bias;     //杈撳嚭鍊�(鍚庨獙浼拌)鐨勫井鍒�=瑙掗�熷害
    return angle;
}

float Complementary_Filter(float angle_m, float gyro_m,float K1)
{
     static float angle;
     angle = K1 * angle_m+ (1-K1) * (angle + (gyro_m-0.2) * dt);
     return angle;
}


void euler_deg2rad(attitude* deg,attitude* rad)
{
    rad->Pitch=deg->Pitch*DEG2RAD;
    rad->Roll=deg->Roll*DEG2RAD;
    rad->Yaw=deg->Yaw*DEG2RAD;
}
void Get_euler(void)
{
    READ_MPU963();
    Angle_x_temp=atan2(accf.y,accf.z)*RAD2DEG;     //璁＄畻鍊捐锛岃浆鎹㈠崟浣嶄负搴�
    Angle_y_temp=atan2(accf.x,accf.z)*RAD2DEG;     //璁＄畻鍊捐锛岃浆鎹㈠崟浣嶄负搴�
    euler_deg.Roll=Kalman_Filter_x(Angle_y_temp,-gyrof.y);//鍗″皵鏇兼护娉�
    euler_deg.Yaw+=gyrof.z*0.002;
    if(euler_deg.Yaw>180)euler_deg.Yaw-=360;
    if(euler_deg.Yaw<-180)euler_deg.Yaw+=360;

}





float fast_sqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *) &i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}



#define HalfT     0.001f  //2ms鐠侊紕鐣绘稉锟藉▎锟�

float m_integralErrorX, m_integralErrorY, m_integralErrorZ;  // 鐠囶垰妯婄粔顖氬瀻
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;
float AHRS_Kp = 1.0;   // 閸旂娀锟界喎瀹崇拋锛勬畱閺�鑸垫殐闁喓宸煎В鏂剧伐婢х偟娉�
float AHRS_Ki = 0.006;  //闂勶拷閾昏桨鍗庨弨鑸垫殐闁喓宸奸惃鍕濋崚鍡楊杻閻╋拷 0.004

//娴滄帟藟濠娿倖灏�
void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az) {
    float vx, vy, vz;    //瑜版挸澧犻惃鍕簚娴ｆ挸娼楅弽鍥╅兇娑撳﹦娈戦柌宥呭閸楁洑缍呴崥鎴﹀櫤
    float ex, ey, ez;    //閸ユ稑鍘撻弫鎷岊吀缁犳锟介棿绗岄崝鐘伙拷鐔峰鐠佲剝绁撮柌蹇擄拷鑲╂畱鐠囶垰妯�

    //鐎电懓濮為柅鐔峰閺佺増宓佹潻娑滎攽瑜版帊绔撮崠锟� 瀵版鍩岄崡鏇氱秴閸旂娀锟界喎瀹�
    float norm = fast_sqrt(ax * ax + ay * ay + az * az);
    ax *= norm;
    ay *= norm;
    az *= norm;

    //閺嶈宓佽ぐ鎾冲閸ユ稑鍘撻弫鎵畱婵寧锟戒礁锟藉吋娼垫导鎵暬閸戝搫鎮囬柌宥呭閸掑棝鍣洪妴鍌滄暏娴滃骸鎷伴崝鐘伙拷鐔活吀鐎圭偤妾ù瀣櫤閸戠儤娼甸惃鍕倗闁插秴濮忛崚鍡涘櫤鏉╂稖顢戠�佃鐦敍灞肩矤閼板苯鐤勯悳鏉款嚠閸ユ稖閰辨慨鎸庯拷浣烘畱娣囶喗顒�
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    //閸欏袧閺夈儴顓哥粻妞惧強缁犳娈戦柌宥呭閸滃苯鐤勯梽鍛ゴ闁插繒娈戦柌宥呭鏉╂瑤琚辨稉顏堝櫢閸旀稑鎮滈柌蹇庣闂傚娈戠拠顖氭▕閵嗭拷
    ex = ay * vz - az * vy;
    ey = az * vx - ax * vz;
    ez = ax * vy - ay * vx;

    //閻€劌寮舵稊妯款嚖瀹割喗娼甸崑姝匢娣囶喗顒滈梽锟介摶娲祩閸嬪骏绱�
    m_integralErrorX += HalfT * ex;
    m_integralErrorY += HalfT * ey;
    m_integralErrorZ += HalfT * ez;

    gx += (AHRS_Kp * ex + AHRS_Ki * m_integralErrorX);
    gy += (AHRS_Kp * ey + AHRS_Ki * m_integralErrorY);
    gz += (AHRS_Kp * ez + AHRS_Ki * m_integralErrorZ);

    /*閺佺増宓佹穱顔筋劀鐎瑰本鍨氶敍灞肩瑓闂堛垺妲搁崶娑樺帗閺佹澘浜曢崚鍡樻煙缁嬶拷*/

    //閸ユ稑鍘撻弫鏉夸簳閸掑棙鏌熺粙瀣剁礉閸忔湹鑵慼alfT娑撶儤绁撮柌蹇撴噯閺堢喓娈�1/2閿涘疅x gy gz娑撴椽妾ч摶杞板崕鐟欐帡锟界喎瀹抽敍灞间簰娑撳鍏橀弰顖氬嚒閻儵鍣洪敍宀冪箹闁插奔濞囬悽銊ょ啊娑擄拷闂冨爼绶抽崫銉ョ氨婵夋梹鐪扮憴锝呮磽閸忓啯鏆熷顔煎瀻閺傚湱鈻�
    q0 += (-q1 * gx - q2 * gy - q3 * gz) * HalfT;
    q1 += ( q0 * gx + q2 * gz - q3 * gy) * HalfT;
    q2 += ( q0 * gy - q1 * gz + q3 * gx) * HalfT;
    q3 += ( q0 * gz + q1 * gy - q2 * gx) * HalfT;

    //normalise quaternion
    norm = fast_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= norm;
    q1 *= norm;
    q2 *= norm;
    q3 *= norm;

    euler_deg.Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.295779513082f; // pitch
    euler_deg.Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.295779513082f; // roll

}
void IMU_get_euler(void)
{
    READ_MPU963();
    Angle_x_temp=atan2(accf.y,accf.z)*RAD2DEG;     //璁＄畻鍊捐锛岃浆鎹㈠崟浣嶄负搴�
    Angle_y_temp=atan2(accf.x,accf.z)*RAD2DEG;     //璁＄畻鍊捐锛岃浆鎹㈠崟浣嶄负搴�
    IMU_AHRSupdate(gyrof.x*DEG2RAD, gyrof.y*DEG2RAD, gyrof.z*DEG2RAD, accf.x, accf.y, accf.z);
    //euler_deg.Yaw+=gyrof.z*0.002;

}
