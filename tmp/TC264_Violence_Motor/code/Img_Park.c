#include"Img_Park.h"
#include"Img.h"

int JumpPoint=0;

void ImgPark_Init(){


}

uint8 ImgPark_RoadJudge(){
    JumpPoint=0;
    int Temp=0;
    if(LeftLoseNum<15&&RightLoseNum<15&&StartTime>20&&ForwardFlag==1){
        int Up,Down;
        for(int i=0;i<60;i++){
                if(LeftLine[i]||RightLine[i])
                {
                    Up=i;
                    break;
                }
        }
        for(int i=59;i>0;i--){
            if(LeftLine[i]||RightLine[i]){
                Down=i;
                break;
            }
        }
        int Point1,Point2,Point3;
        int Step=(Down-Up)/4;
        Point1=Down-Step;
        Point2=Down-2*Step;
        Point3=Down-3*Step;
        int x,y;
        _FUNC_IMG_SEARCHJUMP(Point1)
        _FUNC_IMG_SEARCHJUMP(Point2)
        _FUNC_IMG_SEARCHJUMP(Point3)
        _FUNC_IMG_SEARCHJUMP(55)
        if(JumpPoint>_DEF_IMG_PARKFIELD){
            RoadStat=_DEF_IMG_PARKLEFT;
            FactorStat=_DEF_IMG_PARKBEGIN;
            StartTime=0;
            return 1;
        }
    }
    return 0;
}

void ImgPark_FactorJudge(){
   if(StartTime>8){
        Motor.brake=1;
        RoadStat=FactorStat=0;

    }


}
void ImgPark_Action(){
    ImgPark_FactorJudge();

    //ImgPark_RoadJudge();
}
