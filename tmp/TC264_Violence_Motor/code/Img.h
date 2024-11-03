#include "zf_common_headfile.h"
#include "Img_Base.h"
#include "Img_Error.h"
#include "Img_Cross.h"
#include "Img_Config.h"
#include "Img_Island.h"
#include "Img_Park.h"
#include "led.h"
#include "pid.h"
#ifndef _IMG_H
#define _IMG_H
//struct pid;

extern unsigned short Img_VisitFlag[_DEF_IMG_HEIGHT][_DEF_IMG_WIDTH];
extern int LeftLine[60];
extern int RightLine[60];
extern int CenterLine[60];
extern int LeftLoseLine[60];
extern int RightLoseLine[60];
extern int BottomRow;
extern int TopRow;
extern int SearchBeginRow;
extern int SearchBeginCol;
extern int LeftLose;
extern int RightLose;
extern int LeftLoseNum;
extern int RightLoseNum;
extern Point LeftUp,LeftDown,RightUp,RightDown;
extern int LeftLineNum[60];
extern int RightLineNum[60];
extern int RoadStat;
extern int FactorStat;
extern int LineWidth[60];
extern int LeftLineType;
extern int RightLineType;
extern double RightLineK;
extern double LeftLineK;
extern int LeftMidLoseNum;
extern int RightMidLoseNum;
extern int ImgShowFlag;
extern int LeftHoleFlag;
extern int RightHoleFlag;
extern int LeftLinePre[60];
extern int RightLinePre[60];
extern int CalWidthFlag;
extern int JumpPoint;
extern int Error_ZeroFlag;
extern double Error_FactorProvide;
extern int UpStart;
extern int Width[60];
extern int ErrUp;
extern int ErrDown;
extern int EndLine;
extern int LeftLoseNumRecent;
extern int RightLoseNumRecent;
extern int RightNodeNum;
extern int LeftNodeNum;
extern int StartTime;
extern int Img_ExposureTime;
extern int Img_RingFlag;
extern int Img_S;
void Img_NormalAction();
void Img_Init();
void Img_Proccess();
void Img_JudgeFactor();

#endif
