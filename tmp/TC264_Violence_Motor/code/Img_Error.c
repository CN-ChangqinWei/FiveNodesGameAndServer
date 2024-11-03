#include"Img_Error.h"
#include"Img.h"

#define _DEF_MINEST (0.001)
double B=0,A=0;
double Error=0;
double Error_dot=0;
double Error_dot_dot=0;
void Img_CaculateK(int* Line,int startline,int endline){

   double Sumx=0,Sumy=0,Avgx=0,Avgy=0,Sumxy=0,Sumy2=0;
   int num=0,lastpoint=0,tempx=0;
   A=0;
   //num=endline-startline+1;
   for(int i=startline;i<=endline;i++){

       if(Line[i]){
           Sumy+=i;
           Sumx+=Line[i];
       Sumxy+=i*Line[i];
       Sumy2+=i*i;
       num++;

       //A+=(double)(Line[i]-_DEF_IMG_MID);
       }


   }
   if(num){

        Avgy=Sumy/num;
   Avgx=Sumx/num;
   for(int i=startline;i<=endline;i++){
          if(Line[i]){
              tempx+=(Line[i]-Avgx)*(Line[i]-Avgx);
          }
   }
   tempx/=num;
   Img_S=tempx;
   double up,down;
   up=(Sumxy-num*Avgy*Avgx);
   down=(Sumy2-num*Avgy*Avgy);
   if(down==0)  down=_DEF_MINEST;
   B=up/down;
   A=Sumx-(double)num*(double)_DEF_IMG_MID;
   A=A/(double)num;
   }


   //return k;

}

void ImgError_GernerateErr(int* Line,int startline,int endline){
    static double lastError=0,lastlastError=0,RemainError=0,Detal=0.01,RemainFlag=0;
    Img_CaculateK(Line,startline,endline);
   // if(CalWidthFlag)
//    for(int i=0;i<60;i++){
//        if(LeftLine[i]&&RightLine[i])
//        LineWidth[i]=(RightLine[i]-LeftLine[i])/2;
//    }

    double Errortmp=0;
//    if(_DEF_IMG_PODAO==RoadStat){
//        //Errortmp=(0.8*A+0.2*B)*2;
//        Errortmp=0;
//
//    }else{
    Errortmp=(0.8*A+0.2*B)*2*0.2+0.8*lastError;

//    Error_dot_dot=lastError-lastlastError;
//Error_dot=Errortmp-lastError;
    //Error=lastError;
    //if(RoadStat==_DEF_IMG_CROSS&&_FUNC_ABS((Error_dot))>60) Error=lastError;
//    if(RoadStat==_DEF_IMG_CROSS&&_FUNC_ABS((Error_dot))>40){
//        RemainError=lastError;
//        RemainFlag=1;
//
//
//    }
//    if(RemainFlag&&RoadStat==_DEF_IMG_CROSS){
//        if(_FUNC_ABS(RemainError-Errortmp)<10){
//            RemainFlag=0;
//
//        }else if(RemainError-Errortmp>0){
//            RemainError-=Detal;
//            Errortmp=RemainError;
//        }else{
//            RemainError+=Detal;
//            Errortmp=RemainError;
//
//        }
//
//
//    }else RemainFlag=0;
    //if(Error_ZeroFlag) Error=Error_FactorProvide;

    Error=Errortmp;
    if(Error<-15) direction=left;
    else if(Error>15) direction=right;
    else direction=forward;

    lastlastError=lastError;
    lastError=Error;



}

double ImgError_CaculateK(int*Line,int startline,int endline ){


    double Sumx=0,Sumy=0,Avgx=0,Avgy=0,Sumxy=0,Sumy2=0;
       double num=0;

       for(int i=startline;i>=endline;i--){

           if(Line[i]){
               Sumy+=i;
               Sumx+=Line[i];
               Sumxy+=i*Line[i];
               Sumy2+=i*i;
               num+=1;
           }
       }
       double k=0;
       if(num){
           Avgy=Sumy/num;
        Avgx=Sumx/num;
        double up,down;
        up=(Sumxy-num*Avgy*Avgx);
        down=(Sumy2-num*Avgy*Avgy);
        if(down==0) down=_DEF_MINEST;
        k=up/down;
       }
       return k;
}

void ImgError_Proccess(){



}

