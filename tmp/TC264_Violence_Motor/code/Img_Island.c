#include"Img_Island.h"
#include"Img.h"
IMG_FUNC ImgIslandActions[_DEF_IMG_NOFISLANDFACTOR]={0};


void ImgIsland_Init(){

//    void ImgIsland_FactorJudge();
//
//    void ImgIsland_Action();
//
//    void ImgIsland_Pre();
//
//    void ImgIsland_Enter();
//
//    void ImgIsland_InRing();
//
//    void ImgIsland_Out();
//
//    void ImgIsland_End();
     ImgIslandActions[_DEF_IMG_ISLANDPRE]=ImgIsland_Pre;
     ImgIslandActions[_DEF_IMG_ISLANDENTER]=ImgIsland_Enter;
     ImgIslandActions[_DEF_IMG_ISLANDPREINRING]=ImgIsland_PreInRing;
     ImgIslandActions[_DEF_IMG_ISLANDINRING]=ImgIsland_InRing;
     ImgIslandActions[_DEF_IMG_ISLANDOUT]=ImgIsland_Out;
     ImgIslandActions[_DEF_IMG_ISLANDPREEND]=ImgIsland_PreEnd;
     ImgIslandActions[_DEF_IMG_ISLANDEND]=ImgIsland_End;
}

uint8 ImgIsland_RoadJudge(){

    if(RightLoseNum<10&&LeftLoseNum>10&&RightLineK<_DEF_IMG_STRAGHT&&RightDown.x==0&&RightUp.x==0&&LeftDown.x&&EndLine<=11&&LeftNodeNum>=50){
        RoadStat=_DEF_IMG_ISLANDLEFT;
        FactorStat=_DEF_IMG_ISLANDPRE;
        CalWidthFlag=0;
        return 1;
    }else if(LeftLoseNum<10&&RightLoseNum>10&&LeftLineK<_DEF_IMG_STRAGHT&&LeftDown.x==0&&LeftUp.x==0&&RightDown.x&&EndLine<=11&&RightNodeNum>=50){
        CalWidthFlag=0;
        RoadStat=_DEF_IMG_ISLANDRIGHT;
        FactorStat=_DEF_IMG_ISLANDPRE;
        return 1;
    }
    return 0;
}

void ImgIsland_FactorJudge(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
        if(FactorStat==_DEF_IMG_ISLANDPRE){
            //if(ImgPark_RoadJudge()) return;
                        if(ImgCross_RoadJudge())   return ;
            if(EndLine>11||RightUp.x||RightDown.x||LeftLoseNum<5&&RightLoseNum<5&&RightLineK<_DEF_IMG_STRAGHT&&LeftLineK<_DEF_IMG_STRAGHT)
            {
                RoadStat=_DEF_IMG_NORMAL;
                FactorStat=0;
            }else if(/*LeftLineK>_DEF_IMG_STRAGHT&&*/LeftDown.x==0&&LeftLoseNumRecent<10){
                FactorStat=_DEF_IMG_ISLANDENTER;
                element=cirque;
            }
        }else if(FactorStat==_DEF_IMG_ISLANDENTER){

            if(RightLoseNum>30){
                FactorStat=_DEF_IMG_ISLANDPREINRING;
            }
        }else if(FactorStat==_DEF_IMG_ISLANDPREINRING){
            if(RightLoseNum<5){
                FactorStat=_DEF_IMG_ISLANDINRING;
            }
        }
        else if(FactorStat==_DEF_IMG_ISLANDINRING){
            if(RightLoseNum>10){
                FactorStat=_DEF_IMG_ISLANDOUT;
            }
        }else if(FactorStat==_DEF_IMG_ISLANDOUT){
            if(RightLoseNum<=0){
               FactorStat=_DEF_IMG_ISLANDPREEND;
            }
        }else if(FactorStat==_DEF_IMG_ISLANDPREEND){
            if(LeftLoseNum>10)
                FactorStat=_DEF_IMG_ISLANDEND;
        }
        else if(FactorStat==_DEF_IMG_ISLANDEND){
            if(LeftLoseNum<5){
                RoadStat=FactorStat=0;
            }
        }
    }else{

        if(FactorStat==_DEF_IMG_ISLANDPRE){
            //if(ImgPark_RoadJudge()) return;
            if(ImgCross_RoadJudge())   return ;
                   if(EndLine>11||LeftUp.x||LeftDown.x||LeftLoseNum<5&&RightLoseNum<5&&RightLineK<_DEF_IMG_STRAGHT&&LeftLineK<_DEF_IMG_STRAGHT)
                    {
                        RoadStat=_DEF_IMG_NORMAL;
                        FactorStat=0;
                    }else if(/*RightLineK>_DEF_IMG_STRAGHT&&*/RightDown.x==0&&RightLoseNumRecent<10){
                        FactorStat=_DEF_IMG_ISLANDENTER;
                        element=cirque;
                    }}
                else if(FactorStat==_DEF_IMG_ISLANDENTER){
                    if(LeftLoseNum>30){
                        FactorStat=_DEF_IMG_ISLANDPREINRING;
                    }
                }else if(FactorStat==_DEF_IMG_ISLANDPREINRING){
                    if(LeftLoseNum<=0){
                        FactorStat=_DEF_IMG_ISLANDINRING;
                    }
                }else if(FactorStat==_DEF_IMG_ISLANDINRING){
                    if(LeftLoseNum>10){
                        FactorStat=_DEF_IMG_ISLANDOUT;
                    }
                }
                else if(FactorStat==_DEF_IMG_ISLANDOUT){
                    if(LeftLoseNum<5){
                       FactorStat=_DEF_IMG_ISLANDPREEND;
                    }
                }else if(FactorStat==_DEF_IMG_ISLANDPREEND){
                    if(RightLoseNum>10)
                        FactorStat=_DEF_IMG_ISLANDEND;
                }else if(FactorStat==_DEF_IMG_ISLANDEND){

                    if(RightLoseNum<5){
                        RoadStat=FactorStat=0;
                    }
                }

    }

}

void ImgIsland_Action(){
    if(ImgBase_PodaoJudge()) return;
    ImgIsland_FactorJudge();
    if(RoadStat==_DEF_IMG_ISLANDLEFT||RoadStat==_DEF_IMG_ISLANDRIGHT)
    if(ImgIslandActions[FactorStat]){

        (*ImgIslandActions[FactorStat])();

    }
}

void ImgIsland_Pre(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
        //ImgBase_FixCorner(_DEF_IMG_LEFT);
        _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,1);
    }else{
        //ImgBase_FixCorner(_DEF_IMG_RIGHT);
        _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,1);
    }

}

void ImgIsland_Enter(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
        _DEF_IMG_ALONGONELINEISLAND2(Left,59,0,_DEF_IMG_LEFT,1);

        }else{

            _DEF_IMG_ALONGONELINEISLAND2(Right,59,0,_DEF_IMG_RIGHT,1);

        }

}
void ImgIsland_PreInRing(){
    if(RoadStat==_DEF_IMG_ISLANDLEFT){
        _DEF_IMG_ALONGONELINEISLAND2(Left,59,0,_DEF_IMG_LEFT,0.8);

            }else{

                _DEF_IMG_ALONGONELINEISLAND2(Right,59,0,_DEF_IMG_RIGHT,0.8);

            }
}
void ImgIsland_InRing(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
        //ImgBase_FixCorner(_DEF_IMG_LEFT);
        _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,1);
       }else{
           //ImgBase_FixCorner(_DEF_IMG_RIGHT);
           _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,1);
       }
}

void ImgIsland_Out(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
           _DEF_IMG_ALONGONELINEISLAND2(Left,59,0,_DEF_IMG_LEFT,1);

               }else{

                   _DEF_IMG_ALONGONELINEISLAND2(Right,59,0,_DEF_IMG_RIGHT,1);

               }
}
void ImgIsland_PreEnd(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
               _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,0.8);
           }else{
               _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,0.8);
           }

}
void ImgIsland_End(){

    if(RoadStat==_DEF_IMG_ISLANDLEFT){
                  _DEF_IMG_ALONGONELINE(Right,59,0,_DEF_IMG_RIGHT,0.8);
              }else{
                  _DEF_IMG_ALONGONELINE(Left,59,0,_DEF_IMG_LEFT,0.8);
              }

}


