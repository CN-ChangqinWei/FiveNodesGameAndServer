/*
 * RectChange.c
 *
 *  Created on: 2023Äê11ÔÂ21ÈÕ
 *      Author: howlo
 */
#include"RectChange.h"
#include"math.h"
//#define 2PI 6.2831852
#define PI 3.1415926
#define Delta(x) ((x/360.0)*2PI)
#define PRESSIMG_W 94
#define PRESSIMG_H 60
uint8* ReflactMap[PRESSIMG_H][PRESSIMG_W]={0};

float Rect1[3][3]=
{0};
uint8 BLACK=0;
double RectUn[3][3]={
        {-0.962686,0.910059,-44.829611},{0.115658,0.204103,-53.223244},{0.001134,0.010405,-1.093285}
};

void RectChanging(double (*rect)[3])
{
#ifdef  _DEF_RECTCHANG
    int i,j,x,y;
    //memset(ReflactMap,&BLACK,sizeof(ReflactMap));
    for(i=0;i<PRESSIMG_H;i++)
        for(j=0;j<PRESSIMG_W;j++)
            ReflactMap[i][j]=&BLACK;

    for(i=0;i<PRESSIMG_W*2;i++)
    {
        for(j=0;j<PRESSIMG_H*2;j++)
        {


            x=(int)(i*rect[0][0]+j*rect[0][1]+rect[0][2]);
            y=(int)(i*rect[1][0]+j*rect[1][1]+rect[1][2]);
            if(x>=0&&x<PRESSIMG_W&&y>=0&&y<PRESSIMG_H)
                ReflactMap[y][x]=&mt9v03x_image[j][i];


        }
    }
#else

    for(int i=0;i<PRESSIMG_H;i++)
        for(int j=0;j<PRESSIMG_W;j++)
            ReflactMap[i][j]==&mt9v03x_image[i][j];




#endif
    return;

}

double AngleToTheOne(double angle){

    double Angle;
    Angle=angle-(int)(angle/(2*PI));
    return Angle;

}

void RectChanging_QICI(double (*rect)[3]){
#ifdef  _DEF_RECTCHANGE
    int i,j,x,y;
    for(i=0;i<PRESSIMG_H;i++)
           for(j=0;j<PRESSIMG_W;j++)
               ReflactMap[i][j]=&BLACK;
       for(i=0;i<PRESSIMG_W;i++)
       {
           for(j=0;j<PRESSIMG_H;j++)
           {


               x=(int)((i*rect[0][0]+j*rect[0][1]+rect[0][2])/(i*rect[2][0]+j*rect[2][1]+rect[2][2]));
               y=(int)((i*rect[1][0]+j*rect[1][1]+rect[1][2])/(i*rect[2][0]+j*rect[2][1]+rect[2][2]));
               if(x>=0&&x<PRESSIMG_W*2&&y>=0&&y<PRESSIMG_H*2)
                   ReflactMap[j][i]=&mt9v03x_image[y][x];


           }
       }

#else
       for(int i=0;i<PRESSIMG_H;i++)
              for(int j=0;j<PRESSIMG_W;j++)
                  ReflactMap[i][j]=&mt9v03x_image[i*2][j*2];
#endif
       return;

}

void RectCal(float (*RectL)[3],float (*RectR)[3],float (*RectResult)[3])
{
    int i,j;
    for(i=0;i<3;i++){

        for(j=0;j<3;j++){

            RectResult[i][j]=RectL[i][0]*RectR[0][j]+RectL[i][1]*RectR[1][j]+RectL[i][2]*RectR[2][j];

        }

    }
}


void RectRoll(float Alpha,float Beta,float Gama)
{

    float Rect1[3][3]={0};
    float Rect2[3][3]={0};
    float Rect3[3][3]={0};





}

void RectInit(){

    RectChanging_QICI(RectUn);
}

