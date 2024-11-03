#include "Img_Cross.h"
#include "Img.h"
#include "Img_Base.h"


 IMG_FUNC ImgCrossActions[2]={0};

void ImgCross_Init(){

    ImgCrossActions[_DEF_IMG_CROSSBEGIN]=ImgCross_Begin;
    ImgCrossActions[_DEF_IMG_CROSSUPPOINT]=ImgCross_Up;

}

uint8 ImgCross_RoadJudge(){
if(LeftDown.x&&RightDown.x&&LeftLoseNum>=10&&RightLoseNum>=10||(LeftLoseNum>20&&RightLoseNum>20)){
    Error_ZeroFlag=0;
        RoadStat=_DEF_IMG_CROSS;
        FactorStat=_DEF_IMG_CROSSBEGIN;
        element=cross;
        return 1;
    }
    return 0;
}

void ImgCross_FactorJudge(){
    if(FactorStat==_DEF_IMG_CROSSBEGIN){
        if(LeftDown.x==0&&RightDown.x==0&&(RightUp.x||LeftUp.x)){
            FactorStat=_DEF_IMG_CROSSUPPOINT;
        }

    }else if(FactorStat==_DEF_IMG_CROSSUPPOINT){
        if(RightLoseNum<5||LeftLoseNum<5||RightUp.x==0&&LeftUp.x==0||RightDown.x||LeftDown.x)
        {
            FactorStat=0;
            RoadStat=0;
            Error_ZeroFlag=0;
        }

    }
}

void ImgCross_Action(){

    ImgCross_FactorJudge();
    (*ImgCrossActions[FactorStat])();
    _FUNC_IMG_CREATECENTERLINE
}

void ImgCross_Begin(){



//    if(_FUNC_ABS(Error)<8) Error_ZeroFlag=1;
//    if(LeftLoseNum<15||RightLoseNum<15) Error_ZeroFlag=0;
//    if(Error_ZeroFlag){
////        if(LeftLoseNum-RightLoseNum>5) Error_FactorProvide=3;
////        else if(RightLoseNum-LeftLoseNum>5) Error_FactorProvide=-3;
//         Error_FactorProvide=Error;
//    }
    if(LeftDown.x&&LeftUp.x&&_FUNC_ABS(LeftDown.x-LeftUp.x)<10){

        ImgBase_LinkPoint(LeftUp,LeftDown,LeftLine);
        _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,1);

    }else if(RightDown.x&&RightUp.x&&_FUNC_ABS(RightDown.x-RightUp.x)<10){
        ImgBase_LinkPoint(RightUp,RightDown,RightLine);
        _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,1);
    }
    else{
        _FUNC_IMG_FIXLINEDOWN(Left)
        _FUNC_IMG_FIXLINEDOWN(Right)
        if(LeftDown.x&&RightDown.x==0||RightLoseNum>30&&LeftDown.x){
               _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,1);
           }else if(LeftUp.x==0&&RightUp.x||LeftLoseNum>30&&RightDown.x){
               _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,1);
           }
    }

}

void ImgCross_Up(){
//    if(_FUNC_ABS(Error)<8) Error_ZeroFlag=1;
//    if(LeftLoseNum<15||RightLoseNum<15) Error_ZeroFlag=0;
//    if(Error_ZeroFlag){
////        if(LeftLoseNum-RightLoseNum>5) Error_FactorProvide=3;
////               else if(RightLoseNum-LeftLoseNum>5) Error_FactorProvide=-3;
//                Error_FactorProvide=Error;
//        }
    //if(LeftUp)
    //if(_FUNC_ABS(LeftUp.x))
    if(Error<0){
        if(RightUp.x-_DEF_IMG_MID>20) RightUp.x=0;

    }else if(Error>0){
        if(LeftUp.x-_DEF_IMG_MID<-20) LeftUp.x=0;

    }
    _FUNC_IMG_FIXLINEUP(Left)
    _FUNC_IMG_FIXLINEUP(Right)
    if(LeftUp.x&&RightUp.x==0){

        _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,1);

    }else if(LeftUp.x==0&&RightUp.x){
        _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,1);

    }

}
