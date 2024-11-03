#include"Img_Base.h"
#include"RectChange.h"
#include"Img.h"
#include"key.h"

 unsigned short Img_VisitFlag[_DEF_IMG_HEIGHT][_DEF_IMG_WIDTH]={0};
 int LeftLine[60]={0};
 int RightLine[60]={0};
 int CenterLine[60]={0};
 int LeftLoseLine[60]={0};
 int RightLoseLine[60]={0};
 int BottomRow=_DEF_IMG_SEARCHBEGIN;
 int TopRow=_DEF_IMG_SEARCHEND;
 int SearchBeginRow=_DEF_IMG_BEGINROW;
 int SearchBeginCol=_DEF_IMG_BEGINCOL;
 int LeftLose=0;
 int RightLose=0;
 int LeftLoseNum=0;
 int RightLoseNum=0;
 int Direction_X[4]={-1,0,1,0};
 int Direction_Y[4]={0,-1,0,1};
 Point LeftUp,LeftDown,RightUp,RightDown;
 int LeftLineNum[60];
 int RightLineNum[60];
 int EndLine=59;
 int LineWidth[60]={
11,11,11,
12,12,12,
13,13,13,
14,14,14,
15,15,15,
16,16,16,
17,17,17,
18,18,18,
19,19,19,
20,20,20,
21,21,21,
22,22,22,
 23,23,23,
 24,24,24,
25,25,25,
26,26,26,
 27,27,27,
28,28,28,
29,29,29,
30,30,30};
// int LineWidth[60]={
// 24,24,24,
// 24,24,24,
// 25,25,25,
// 25,25,25,
// 25,25,25,
// 26,26,26,
// 26,26,26,
// 26,26,26,
// 27,27,27,
// 27,27,27,
// 27,27,27,
// 28,28,28,
// 28,28,28,
//  28,28,28,
//  29,29,29,
// 29,29,29,
//  29,29,29,
//  30,30,30,
// 30,30,30,
// 30,30,30};
 int WidthCalFlag=1;
 int LeftLineType=0;
 int RightLineType=0;
 double RightLineK=0;
 double LeftLineK=0;
 int LeftMidLoseNum=0;
 int RightMidLoseNum=0;
 int LeftHoleFlag=0;
 int RightHoleFlag=0;
 int LeftLinePre[60]={0};
 int RightLinePre[60]={0};
 int CalWidthFlag=1;
 int LeftLoseNumRecent=0;
 int RightLoseNumRecent=0;
 int LeftNodeNum=0;
 int RightNodeNum=0;
unsigned short Rule(int x,int y){
    //如果越界，丢线
    unsigned short res;
    res=_FUNC_IMG_OUTLIMIT(y,x);
    if(res!=0){
        return res;
    }
    //判断八个方向的差和比
    double cha,bi;
    //cha=_FUNC_ABS((double)_DEF_IMG_REFLECTED(y,x)-(double)_DEF_IMG_REFLECTED(y-1,x-1));
    //bi=cha/((double)_DEF_IMG_REFLECTED(y,x)+(double)_DEF_IMG_REFLECTED(y-1,x-1));
    _FUNC_IMG_CHAHEBI(y,x,y-1,x-1)
    _FUNC_IMG_CHAHEBI(y,x,y-1,x)
    _FUNC_IMG_CHAHEBI(y,x,y-1,x+1)
    _FUNC_IMG_CHAHEBI(y,x,y,x-1)
    _FUNC_IMG_CHAHEBI(y,x,y,x+1)
    _FUNC_IMG_CHAHEBI(y,x,y+1,x-1)
    _FUNC_IMG_CHAHEBI(y,x,y+1,x)
    _FUNC_IMG_CHAHEBI(y,x,y+1,x+1)

    return 0;
}
double ImgBase_Sqrt(double value){



}

int Erfen(int left,int right,int target){

       int center=(left+right)/2;
       if(left<=right){

           if(center*center==target){
           return center;
       }

            else if(center*center>target){
           return Erfen(left,center-1,target);
       }else if(center*center<target){
           return Erfen(center+1,right,target);
       }

       }else if(left>right){
           return left-1;

       }

      return center;
   }

int mySqrt(int x) {
        if(x/2>=46340)
        return Erfen(1,46340,x);
        else if(x>1)
        return Erfen(1,x/2,x);
        else return x;
    }


double ImgBase_AngleRaise(int *Line){


    double res=0;
    int i,j;
   //int sum
    for(i=59;i>=0;i--){

       if(Line[i]) break;

   }
    for(j=i;j>=0;j--) if(Line[j]==0) break;
    j+=5;
    int len=i-j+1;
    len/=2;
   double x1,x2,x3;
   double y1,y2,y3;
   double l1,l2,l3;
   if(Line[i]&&Line[i-len]&&Line[i-2*len]){

       x1=(double)Line[i];
       x2=(double)Line[i-len];
       x3=(double)Line[i-len*2];
       y1=(double)i;
       y2=(double)(i-len);
       y3=(double)(i-len*2);
double S=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/2;
   _FUNC_IMG_LINELEN(l1,x1,y1,x2,y2)
   _FUNC_IMG_LINELEN(l2,x2,y2,x3,y3)
   _FUNC_IMG_LINELEN(l3,x3,y3,x1,y1)
   res=4*S/(l1*l2*l3);
   }


  //return S;
   return _FUNC_ABS(res);
}

void ImgBase_WideCaculate(){
    for(int i=0;i<60;i++){
       if(LeftLine[i]&&RightLine[i]){
           LineWidth[i]=(LeftLine[i]+RightLine[i])/2;
       }
    }
    double k;
    k=ImgError_CaculateK(LineWidth,0,59);
    int i;
    for(i=0;i<60;i++){
        if(LineWidth[i]) break;
    }
    double temp=(double)LineWidth[i];
    for(;i>=0;i--){
        LineWidth[i]=(int)temp;
        temp-=k;
    }
    temp=LineWidth[0];
    for(i=0;i<60;i++)
    {
        if(LineWidth[i]==0) LineWidth[i]=(int)temp;
        temp+=k;
    }
}

void ImgBase_GenerateLine(){
    unsigned short result=0;
    memset(Img_VisitFlag,0,sizeof(Img_VisitFlag));
    memset(LeftLine,0,sizeof(LeftLine));
    memset(RightLine,0,sizeof(RightLine));
    memset(CenterLine,0,sizeof(CenterLine));
    memset(LeftLineNum,0,sizeof(LeftLineNum));
    memset(RightLineNum,0,sizeof(RightLineNum));
    memset(RightLoseLine,0,sizeof(RightLoseLine));
    memset(LeftLoseLine,0,sizeof(LeftLoseLine));
    memset(Width,0,sizeof(Width));
    EndLine=59;
    LeftLose=0;
    RightLose=0;
    LeftLoseNum=0;
    RightLoseNum=0;
    LeftMidLoseNum=0;
    RightMidLoseNum=0;
    LeftHoleFlag=0;
    RightHoleFlag=0;
    LeftLoseNumRecent=0;
    RightLoseNumRecent=0;
    RightNodeNum=0;
    LeftNodeNum=0;
    int times=0;
    int LeftTopRow=55;
    int RightTopRow=55;
    //char text[20]={0};
    int x=SearchBeginCol,y=SearchBeginRow;

    while(result=Rule(--x,y)==0);
    x+=1;
    Img_VisitFlag[y][x]=_DEF_IMG_LEFTLINE;
                 LeftLine[y]=x;
    int FootDirect=_DEF_DIRECT_LEFT;
    int ForwardDirect=FootDirect+1;
    //result=0;
    //找左线
    while(1){
        //找到右边界或者上边界的时候结束
        //检查脚下是否殷实
        //if(Img_VisitFlag[])
        //Img_VisitFlag[y][x]=result;
        times++;
        result=Rule(x+Direction_X[FootDirect],y+Direction_Y[FootDirect]);
        EndLine=_FUNC_MIN(EndLine,y);
             if(result==_DEF_IMG_LEFTLIMITCOLOR){
                 if(LeftHoleFlag==1){
                     LeftHoleFlag=2;
                 }
                 Img_VisitFlag[y][x]=_DEF_IMG_LEFTLIMITCOLOR;
                 LeftLoseNum++;
                 if(y>_DEF_IMG_RECENT) LeftLoseNumRecent++;
                 LeftLoseLine[y]=1;
             }

             else  {
                 Img_VisitFlag[y][x]=_DEF_IMG_LEFTLINE;
                 LeftNodeNum++;
                 LeftTopRow=_FUNC_MIN(LeftTopRow,y);
                 LeftLineNum[y]++;
                 if(LeftHoleFlag==0){

                     LeftHoleFlag=1;

                 }
                 if(LeftLoseLine[y]==0&&LeftLine[y]==0)
                     LeftLine[y]=x;
             }

            if(result==0){

            x+=Direction_X[FootDirect];
            y+=Direction_Y[FootDirect];
            //if(Img_VisitFlag[y][x]) break;
            //FootDirect=_FUNC_LIMIT(0,3,(FootDirect-1));
            //ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect-1));
            FootDirect--;
            ForwardDirect--;
            FootDirect=_FUNC_LIMIT(0,3,FootDirect);
            ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);
            continue;
        }
        else if(result==_DEF_IMG_OUTLIMIT||result==_DEF_IMG_RIGHTLIMITCOLOR||times>200){

            break;
        }

        //检查是否可以前进
        result=Rule(x+Direction_X[ForwardDirect],y+Direction_Y[ForwardDirect]);
        if(result==_DEF_IMG_LINE){
//            FootDirect=_FUNC_LIMIT(0,3,(FootDirect+1));
//            ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect+1));
            FootDirect++;
            ForwardDirect++;
            FootDirect=_FUNC_LIMIT(0,3,FootDirect);
            ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);

        }else if(result==_DEF_IMG_LEFTLIMITCOLOR){
//            FootDirect=_FUNC_LIMIT(0,3,(FootDirect+1));
//            ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect+1));
            LeftLose=1;
            LeftLoseNum++;
            FootDirect++;
            ForwardDirect++;
            FootDirect=_FUNC_LIMIT(0,3,FootDirect);
            ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);
        }else if(result==_DEF_IMG_OUTLIMIT||result==_DEF_IMG_RIGHTLIMITCOLOR){

            break;
        }else{
            x+=Direction_X[ForwardDirect];
            y+=Direction_Y[ForwardDirect];
            //if(Img_VisitFlag[y][x]) break;
        }
    }
        times=0;

        x=SearchBeginCol,y=SearchBeginRow;
        while(result=Rule(++x,y)==0);
            x-=1;

            FootDirect=_DEF_DIRECT_RIGHT;
            ForwardDirect=FootDirect-1;
        while(1){
               //找到左边界或者上边界的时候结束
               //检查脚下是否殷实
               //if(Img_VisitFlag[])
            times++;
            EndLine=_FUNC_MIN(EndLine,y);
result=Rule(x+Direction_X[FootDirect],y+Direction_Y[FootDirect]);

            if(result==_DEF_IMG_RIGHTLIMITCOLOR){
                if(RightHoleFlag==1){
                                    RightHoleFlag=2;
                                }
                Img_VisitFlag[y][x]=_DEF_IMG_RIGHTLIMITCOLOR;
                RightLoseNum++;
                if(y>_DEF_IMG_RECENT) RightLoseNumRecent++;
                RightLoseLine[y]=1;
                //Left_FUNC_MIN
            }

                         else  {
                             if(RightHoleFlag==0){
                                 RightHoleFlag=1;

                             }
                             RightTopRow=_FUNC_MIN(RightTopRow,y);
                             Img_VisitFlag[y][x]=_DEF_IMG_RIGHTLINE;
                             RightLineNum[y]++;
                             RightNodeNum++;
                             if(RightLoseLine[y]==0&&RightLine[y]==0)
                                 RightLine[y]=x;
                         }

               if(result==0){

                   x+=Direction_X[FootDirect];
                   y+=Direction_Y[FootDirect];
                   //if(Img_VisitFlag[y][x]) break;
                   //FootDirect=_FUNC_LIMIT(0,3,(FootDirect-1));
                   //ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect-1));
                   FootDirect++;
                   ForwardDirect++;
                   FootDirect=_FUNC_LIMIT(0,3,FootDirect);
                   ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);
                   continue;
               }
               else if(result==_DEF_IMG_OUTLIMIT||result==_DEF_IMG_LEFTLIMITCOLOR||times>200){

                   break;
               }

               //检查是否可以前进
               result=Rule(x+Direction_X[ForwardDirect],y+Direction_Y[ForwardDirect]);
               if(result==_DEF_IMG_LINE){
       //            FootDirect=_FUNC_LIMIT(0,3,(FootDirect+1));
       //            ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect+1));
                   FootDirect--;
                   ForwardDirect--;
                   //RightLose=1;
                   //RightLoseNum++;
                   FootDirect=_FUNC_LIMIT(0,3,FootDirect);
                   ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);

               }else if(result==_DEF_IMG_RIGHTLIMITCOLOR){
       //            FootDirect=_FUNC_LIMIT(0,3,(FootDirect+1));
       //            ForwardDirect=_FUNC_LIMIT(0,3,(ForwardDirect+1));
                   FootDirect--;
                   ForwardDirect--;
                   FootDirect=_FUNC_LIMIT(0,3,FootDirect);
                   ForwardDirect=_FUNC_LIMIT(0,3,ForwardDirect);
               }else if(result==_DEF_IMG_OUTLIMIT||result==_DEF_IMG_LEFTLIMITCOLOR){

                   break;
               }else{
                   x+=Direction_X[ForwardDirect];
                   y+=Direction_Y[ForwardDirect];
                   //if(Img_VisitFlag[y][x]) break;
               }
//        sprintf(text,"(%d,%d)",x,y);
//        ips200_show_string(0,240,text);


    }
        TopRow=_FUNC_MAX(LeftTopRow,RightTopRow);
        ImgBase_GeneratePoint();

        _FUNC_IMG_CREATECENTERLINE
    for(int i=59;i>TopRow;i--){

        if(CenterLine[i]){
            //SearchBeginRow=i;
            SearchBeginCol=CenterLine[i];
            break;
        }

    }
//        if(BUTTON_1==Key.Trig){
//
//               ImgBase_WideCaculate();
//
//               WidthCalFlag=0;
//           }
        //LeftLineType=ImgBase_AngleRaise(LeftLine);
        //RightLineType=ImgBase_AngleRaise(RightLine);
        RightLineK=ImgBase_AngleRaise(RightLine);
        LeftLineK=ImgBase_AngleRaise(LeftLine);
}
void ImgBase_ShowDealImg(int x,int y){}

void ImgBase_ShowRawImg(int x,int y){}

void ImgBase_GeneratePoint(){

    memset(&LeftUp,0,sizeof(Point));
    memset(&LeftDown,0,sizeof(Point));
    memset(&RightUp,0,sizeof(Point));
    memset(&RightDown,0,sizeof(Point));
    int ddx;
    _FUNC_IMG_JUDGEDOWNPOINT(Left);
    _FUNC_IMG_JUDGEDOWNPOINT(Right);
    _FUNC_IMG_JUDGEUPPOINT(Left)
    _FUNC_IMG_JUDGEUPPOINT(Right)

//    if(LeftUp.x){
//
//        int End=LeftUp.y;
//        int KStart=LeftUp.y;
//        int KEnd=LeftUp.y-5;
//        int Start=_DEF_IMG_SEARCHBEGIN;
//        ImgBase_FixLine(KStart,KEnd,Start,End,LeftLine,_DEF_IMG_UP);
//    }

    //_FUNC_IMG_FIXLINEUP(Left)
    //_FUNC_IMG_FIXLINEUP(Right)



}



void ImgBase_FixLine(int startrowk,int endrowk,int startrow,int endrow,int* Line,int Mode){
    double k,temp;

    //ips200_show_float(0,300,(float)k,3,3);
    if(Mode==_DEF_IMG_UP){
        temp=(double)Line[endrow];
        k=ImgError_CaculateK(Line,startrowk,endrowk);
        for(int i=endrow+1;i<=startrow;i++){
                    temp+=k;
                    if(_FUNC_IMG_OUTX(temp)) Line[i]=(int)(temp);
                    else{
                    Line[i]=(int)(temp);
                    Img_VisitFlag[i][Line[i]]=_DEF_IMG_LINE;
                    }
                    }

    }else{
        temp=(double)Line[startrow];
        k=ImgError_CaculateK(Line,startrowk,endrowk);
        for(int i=startrow-1;i>=endrow;i--){
          temp-=k;
          if(_FUNC_IMG_OUTX(temp)) Line[i]=(int)(temp);
                              else{
                              Line[i]=(int)(temp);
                              Img_VisitFlag[i][Line[i]]=_DEF_IMG_LINE;
                              }

        }
        //ImgError_CaculateK(Line,startrowk,endrowk);

    }

}

void ImgBase_FixCorner(int Direction){
    int temp;
    if(Direction==_DEF_IMG_LEFT){

        for(int i=_DEF_IMG_SEARCHBEGIN;i>_DEF_IMG_SEARCHEND;i--){
            if(Img_VisitFlag[i][_DEF_IMG_LEFTLIMIT+1]&&RightLine[i]){
                temp=RightLine[i]-(LineWidth[i])*_DEF_IMG_CORNERK;
                if(!_FUNC_IMG_OUTX(temp))
                {
                    CenterLine[i]=temp;
                //Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
                }
                else{
                                   CenterLine[i]=temp;
                     }
            }

        }
    }
    else{

        for(int i=_DEF_IMG_SEARCHBEGIN;i>_DEF_IMG_SEARCHEND;i--){
                    if(Img_VisitFlag[i][_DEF_IMG_RIGHTLIMIT-1]&&LeftLine[i]){
                        temp=LeftLine[i]+(LineWidth[i])*_DEF_IMG_CORNERK;
                        if(!_FUNC_IMG_OUTX(temp))
                        {
                            CenterLine[i]=temp;
                        //Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
                        }
                        else{
                                           CenterLine[i]=temp;
                             }
                    }

                }
}
}
uint8 ImgBase_CornerJudeg()
{
    if(LeftLineK>_DEF_IMG_BENDFIELD&&LeftLoseNum-RightLoseNum>7||LeftLoseNum>20){

        RoadStat=_DEF_IMG_CORNER;
        FactorStat=_DEF_IMG_LEFT;
        return 1;
    }
    if(RightLineK>_DEF_IMG_BENDFIELD&&RightLoseNum-LeftLoseNum>7||RightLoseNum>20)
    {
        RoadStat=_DEF_IMG_CORNER;
        FactorStat=_DEF_IMG_RIGHT;
                return 1;
    }

    return 0;

}

void ImgBase_CornerFactorJudge(){

    if(_FUNC_ABS(LeftLoseNum-RightLoseNum)<5)
    {
        RoadStat=0;
        FactorStat=0;

    }
}

void ImgBase_ConerActions(){
    ImgBase_CornerFactorJudge();
    ImgBase_FixCorner(FactorStat);
}

uint8 ImgBase_PodaoJudge(){

    if(euler_deg.Roll>4){
        Angle_Target=_DEF_DEFAULT_TARGETANGLE-1;
        RoadStat=_DEF_IMG_PODAO;
        return 1;
    }return 0;
}

void ImgBase_PodaoAction(){

    ImaBase_PodaoFactorJudge();
    if(LeftLoseNum>RightLoseNum)
       ImgBase_FixCorner(_DEF_IMG_LEFT);
        else
       ImgBase_FixCorner(_DEF_IMG_RIGHT);
//    int i;
//    for(i=59;i>40;i--){
//        if(Width[i]) break;
//
//    }
//    if(Width[i]){
//
//        for(;i>23;i--){
//            if(Width[i-3]){
//
//                if(Width[i]-Width[i-3]>8){
//                    if(LeftLine[i-3]&&RightLine[i-3]){
//                        LeftUp.y=RightUp.y=i-3;
//                        LeftUp.x=LeftLine[i-3];
//                        RightUp.x=RightLine[i-3];
//                        _FUNC_IMG_FIXLINEUP(Left)
//                        _FUNC_IMG_FIXLINEUP(Right)
//                        _FUNC_IMG_CREATECENTERLINE
//                        break;
//                    }
//                }
//
//            }else break;
//
//        }
//
//    }
}

void ImaBase_PodaoFactorJudge(){
//    if(FactorStat==0){
//    if(euler_deg.Roll>4){
//        FactorStat=1;
//    }}
//    else if(FactorStat==1){
//        if(euler_deg.Roll<3){
//            RoadStat=0;
//            FactorStat=0;
//        Angle_Target=_DEF_DEFAULT_TARGETANGLE;
//
//        }
//
//    }
    if(euler_deg.Roll<0){
                RoadStat=0;
                FactorStat=0;
            Angle_Target=_DEF_DEFAULT_TARGETANGLE;}
}
void ImgBase_LinkPoint(Point Up,Point Down,int* Line){
    if(Up.y<Down.y){
        float K=0;
        float X=Up.x;
        float upnum=Down.x-Up.x;
        float downnum=Down.y-Up.y;
        if(downnum!=0){
            K=upnum/downnum;
        }
        for(int i=Up.y;i<Down.y;i++){
            Line[i]=(int)X;
            Img_VisitFlag[i][(int)X]=_DEF_IMG_LINE;
            X+=K;
        }
    }
}

