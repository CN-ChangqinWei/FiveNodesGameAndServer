
#include"RectChange.h"
#ifndef _IMG_CONFIG_H
#define _IMG_CONFIG_H
#define _DEF_IMG_LEFT    (1)

#define _DEF_IMG_RIGHT   (0)

#define _DEF_IMG_MID    (47)

#define _DEF_IMG_LINE       (RGB565_CYAN)

#define _DEF_IMG_POINTFIELD     (4)

#define _DEF_IMG_POINTCOLOR     (RGB565_CYAN)

#define _DEF_IMG_CHECKLINE      (RGB565_WHITE)

#define _DEF_IMG_UP         (1)

#define _DEF_IMG_DOWN       (0)

#define _DEF_IMG_SEARCHBEGIN    (58)

#define _DEF_IMG_SEARCHEND      (10)

#define _DEF_IMG_LEFTLIMIT      (5)

#define _DEF_IMG_RIGHTLIMIT     (88)

#define _FUNC_IMG_OUTX(x)       (x<=_DEF_IMG_LEFTLIMIT?(1):(x>=_DEF_IMG_RIGHTLIMIT?(1):(0)))

#define _FUNC_IMG_OUTY(y)       (y>=_DEF_IMG_SEARCHBEGIN?(1):(y<=_DEF_IMG_SEARCHEND?(1):0))

#define _FUNC_IMG_SETBIGPOINT(x,y,color,img)    \
    img[y-1][x-1]=color;\
               img[y-1][x]=color;\
               img[y-1][x+1]=color;\
               img[y][x-1]=color;\
               img[y][x]=color;\
               img[y][x+1]=color;\
               img[y+1][x-1]=color;\
               img[y+1][x]=color;\
               img[y+1][x+1]=color;


//Img

#define _DEF_IMG_NORMAL         (0)

#define _DEF_IMG_CROSS          (1)

#define _DEF_IMG_CORNER         (2)

#define _DEF_IMG_ISLANDLEFT     (3)

#define _DEF_IMG_ISLANDRIGHT    (4)

#define _DEF_IMG_PARKLEFT       (5)

#define _DEF_IMG_PARKRIGHT      (6)

#define _DEF_IMG_PODAO          (7)

#define _DEF_IMG_STOP           (8)

#define _DEF_IMG_NOFFACTORS     (9)



#define _FUNC_MAX(a,b)  (a>b?a:b)

#define _FUNC_MIN(a,b)  (a<b?a:b)

typedef void(*IMG_FUNC)();
//ImgPark

#define _DEF_IMG_PARKFIELD      (10)

#define _DEF_IMG_PARKBEGIN      (0)

#define _DEF_IMG_PARKEND        (1)

#define _FUNC_IMG_SEARCHJUMPLEFT(Point) \
        if(_FUNC_IMG_OUTX(RightLine[Point])==0&&_FUNC_IMG_OUTY(Point)==0){\
                   Temp=0;\
                   x=_DEF_IMG_LEFTLIMIT+1;\
                   y=Point;\
                   while(x<RightLine[Point]){\
                       Img_VisitFlag[Point][x]=_DEF_IMG_CHECKLINE;\
                       if(Rule(x,y)==_DEF_IMG_LINE) {Temp++;}\
                       x++;\
                   }\
                   JumpPoint=_FUNC_MAX(Temp,JumpPoint);\
               }

#define _FUNC_IMG_SEARCHJUMPRIGHT(Point) \
        if(_FUNC_IMG_OUTX(LeftLine[Point])==0&&_FUNC_IMG_OUTY(Point)==0){\
                   Temp=0;\
                   x=_DEF_IMG_RIGHTLIMIT-1;\
                   y=Point;\
                   while(x>LeftLine[Point]){\
                       Img_VisitFlag[Point][x]=_DEF_IMG_CHECKLINE;\
                       if(Rule(x,y)==_DEF_IMG_LINE) {Temp++;}\
                       x--;\
                   }\
                   JumpPoint=_FUNC_MAX(Temp,JumpPoint);\
               }

#define _FUNC_IMG_SEARCHJUMP(Point) \
        if(_FUNC_IMG_OUTX(RightLine[Point])==0&&_FUNC_IMG_OUTY(Point)==0&&_FUNC_IMG_OUTX(LeftLine[Point])==0){\
                   Temp=0;\
                   x=LeftLine[Point];\
                   y=Point;\
                   while(x<RightLine[Point]){\
                       Img_VisitFlag[Point][x]=_DEF_IMG_CHECKLINE;\
                       if(Rule(x,y)==_DEF_IMG_LINE) {Temp++;}\
                       x++;\
                   }\
                   JumpPoint=_FUNC_MAX(Temp,JumpPoint);\
               }

//ImgCross
#define _DEF_IMG_CROSSBEGIN     (0)

#define _DEF_IMG_CROSSUPPOINT   (1)

//ImgIsland

#define _DEF_IMG_NOFISLANDFACTOR      (7)

#define _DEF_IMG_ISLANDPRE      (0)

#define _DEF_IMG_ISLANDENTER    (1)

#define _DEF_IMG_ISLANDPREINRING    (2)

#define _DEF_IMG_ISLANDINRING   (3)

#define _DEF_IMG_ISLANDOUT      (4)

#define _DEF_IMG_ISLANDPREEND   (5)

#define _DEF_IMG_ISLANDEND      (6)



#define _DEF_IMG_STRAGHT        (0.004)

//ImgBase
#define _DEF_IMG_CORNERK        (1.3)

#define _DEF_IMG_RECENT         (30)

#define _DEF_IMG_ALONGONELINE(line,start,end,dir,K)\
    for(int i=start;i>end;i--){\
        if(line##Line[i]){   \
            if(dir==_DEF_IMG_LEFT){\
                CenterLine[i]=line##Line[i]+LineWidth[i]*K;\
            }else{\
                CenterLine[i]=line##Line[i]-LineWidth[i]*K;\
            }\
        }\
    }

#define _DEF_IMG_ALONGONECORNER(line,start,end,dir,K)\
    for(int i=start;i>end;i--){\
        if(line##Line[i]){   \
            if(dir==_DEF_IMG_LEFT){\
                CenterLine[i]=line##Line[i]+LineWidth[i]*K;\
            }else{\
                CenterLine[i]=line##Line[i]-LineWidth[i]*K;\
            }\
        }else{\
            if(dir==_DEF_IMG_LEFT){\
                            CenterLine[i]=-1;\
                        }else{\
                            CenterLine[i]=95;\
                        }\
        }\
    }

#define _DEF_IMG_ALONGONELINEISLAND(line,start,end,dir,K)\
        int i;\
        for(i=start;i>end;i--) if(line##Line[i]) break;\
        for(;i>end;i--){\
            if(line##Line[i]){   \
                if(dir==_DEF_IMG_LEFT){\
                    CenterLine[i]=line##Line[i]+LineWidth[i]*K;\
                }else{\
                    CenterLine[i]=line##Line[i]-LineWidth[i]*K;\
                }\
            }else break;\
        }

#define _DEF_IMG_ALONGONELINEISLAND2(line,start,end,dir,K)\
        int i,flag=0;\
        for(i=start;i>end+30;i--) if(line##Line[i]) break;\
        for(;i>end;i--){\
            if(line##Line[i]&&flag==0){   \
                if(dir==_DEF_IMG_LEFT){\
                    CenterLine[i]=line##Line[i]+LineWidth[i]*K;\
                }else{\
                    CenterLine[i]=line##Line[i]-LineWidth[i]*K;\
                }\
            }else if(line##Line[i]==0){flag=1;\
                if(dir==_DEF_IMG_LEFT){\
                                CenterLine[i]=_DEF_IMG_LEFTLIMIT+LineWidth[i]*K;\
                            }else{\
                                CenterLine[i]=_DEF_IMG_RIGHTLIMIT-LineWidth[i]*K;\
                            }\
                }else break;\
        }

#define _DEF_IMG_BENDFIELD      (0.04)

#define _FUNC_IMG_LINELEN(l,x1,y1,x2,y2)  \
    l=mySqrt((int)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)));

#define _DEF_IMG_CORNERFIELD    (3)

//#define _DEF_IMG_STRAGHT        (0)

#define _DEF_IMG_BEND           (1)

#define _FUNC_IMG_CREATECENTERLINE  \
        for(int i=0;i<60;i++){\
                if(LeftLine[i]&&RightLine[i]){\
                    CenterLine[i]=(LeftLine[i]+RightLine[i])/2;\
                    Width[i]=RightLine[i]-LeftLine[i];\
                }\
                else if(LeftLine[i]){\
                    Width[i]=_DEF_IMG_RIGHTLIMIT-LeftLine[i];\
                }\
                else if(RightLine[i]){\
                    Width[i]=RightLine[i]-_DEF_IMG_LEFTLIMIT;\
                } \
            }

#define _FUNC_ABS(x)        ((x)<0?(-(x)):(x))

#define _DEF_IMG_FIELD          (0.1)

#define _DEF_IMG_WIDTH   (94)

#define _DEF_IMG_HEIGHT  (60)


#define _DEF_IMG_REFLECTED(y,x)  (*(ReflactMap[y][x]))



#define _DEF_IMG_LeftLIMITLINE  (_DEF_IMG_LEFTLIMIT+1)

#define _DEF_IMG_RightLIMITLINE (_DEF_IMG_RIGHTLIMIT-1)


#define _DEF_IMG_BEGINROW  (57)

#define _DEF_IMG_BEGINCOL    (47)

#define _DEF_IMG_LEFTLINE           (RGB565_RED)

#define _DEF_IMG_RIGHTLINE          (RGB565_BLUE)

#define _DEF_IMG_CENTERLINE         (RGB565_GREEN)

#define _DEF_IMG_LEFTLIMITCOLOR     (RGB565_YELLOW)

#define _DEF_IMG_OUTLIMIT           (RGB565_PURPLE)

#define _DEF_IMG_RIGHTLIMITCOLOR    (RGB565_PINK)

#define _FUNC_IMG_OUTLIMIT(y,x) (y<_DEF_IMG_SEARCHEND?_DEF_IMG_OUTLIMIT:(y>=_DEF_IMG_SEARCHBEGIN?_DEF_IMG_LINE:(x<=_DEF_IMG_LEFTLIMIT?_DEF_IMG_LEFTLIMITCOLOR:(x>=_DEF_IMG_RIGHTLIMIT?_DEF_IMG_RIGHTLIMITCOLOR:0))))








//#define _DEF_IMG_POINTFIELD (10)

#define _FUNC_IMG_JUDGEDOWNPOINT(line)\
for(int i=_DEF_IMG_BEGINROW-10;i>_DEF_IMG_SEARCHEND+20;i--){\
        if(line##Line[i-3]&&line##Line[i]&&line##Line[i+3]){\
            ddx=line##Line[i-3]-2*line##Line[i]+line##Line[i+3];\
            ddx=_FUNC_ABS(ddx);\
            if(ddx>_DEF_IMG_POINTFIELD||line##LineNum[i-3]>_DEF_IMG_POINTFIELD||line##LineNum[i-5]+line##LineNum[i-4]+line##LineNum[i-3]>_DEF_IMG_POINTFIELD*3){\
                int SearchFlag=0;\
                for(int j=i;j>i-10;j--){\
                    if(line##LoseLine[j]==1){\
                    _FUNC_IMG_SETBIGPOINT(line##Line[i],i,_DEF_IMG_POINTCOLOR,Img_VisitFlag);\
                    line##Down.x=line##Line[i];\
                    line##Down.y=i;\
                    SearchFlag=1;\
                break;}\
                }\
                if(SearchFlag==1) break;\
            }\
        }\
        else if(i<_DEF_IMG_SEARCHEND+20)break;\
    }
#define _FUNC_IMG_JUDGEUPPOINT(line)\
        if(line##Line[_DEF_IMG_SEARCHEND+2]){\
            int SearchFlag=0;\
        for(int i=_DEF_IMG_SEARCHEND+2;i<50&&SearchFlag==0;i++){\
                if(Img_VisitFlag[i][_DEF_IMG_##line##LIMITLINE]||line##LineNum[i-2]+line##LineNum[i-1]+line##LineNum[i]>_DEF_IMG_POINTFIELD*3){\
                    for(int j=i;j<i+5;j++){\
                        if(line##Line[j]==0){\
                            _FUNC_IMG_SETBIGPOINT(line##Line[i-1],i-5,_DEF_IMG_POINTCOLOR,Img_VisitFlag);\
                        line##Up.x=line##Line[i-3];\
                        line##Up.y=i-1;\
                        SearchFlag=1;\
                        break;\
                        }\
                    }\
                    }\
                }\
        }

typedef struct{

        int x;
        int y;


}Point;



#define _FUNC_IMG_CHAHEBI(y,x,y1,x1)    \
     cha=_FUNC_ABS((double)_DEF_IMG_REFLECTED(y,x)-(double)_DEF_IMG_REFLECTED(y1,x1));\
    bi=cha/((double)_DEF_IMG_REFLECTED(y,x)+(double)_DEF_IMG_REFLECTED(y1,x1));\
    if(bi>_DEF_IMG_FIELD){\
        return _DEF_IMG_LINE;\
    }

#define _DEF_DIRECT_LEFT    (0)

#define _DEF_DIRECT_UP      (1)

#define _DEF_DIRECT_RIGHT   (2)

#define _DEF_DIRECT_DOWN    (3)

#define _FUNC_LIMIT(min,max,value)  (value>max?(min):(value<min?(max):(value)))

#define _FUNC_IMG_FIXLINEDOWN(line)\
        if(line##Down.x){\
               int i=line##Down.y;\
               int Start=line##Down.y;\
               int KStart=SearchBeginRow;\
               int End;\
               int KEnd=line##Down.y;\
               for(;i>_DEF_IMG_SEARCHEND;i--){\
                   if(!line##Line[i])\
                   break;\
               }\
               for(;i>_DEF_IMG_SEARCHEND;i--){\
                   if(line##Line[i]){\
                       break;\
                   }\
               }\
               End=i;\
               ImgBase_FixLine(KStart,KEnd,Start,19,line##Line,_DEF_IMG_DOWN);\
           }

#define _FUNC_IMG_FIXLINEUP(line)\
        if(line##Up.x){\
                int End=line##Up.y;\
                int KStart=line##Up.y;\
                int KEnd=line##Up.y-10;\
                int Start=_DEF_IMG_SEARCHBEGIN;\
                ImgBase_FixLine(KStart,KEnd,Start,End,line##Line,_DEF_IMG_UP);\
            }







#endif
