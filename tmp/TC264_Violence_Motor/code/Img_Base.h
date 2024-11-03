#include "zf_common_headfile.h"
#include "RectChange.h"
#include "Img_Config.h"
#include "Img_Error.h"

#ifndef _IMG_BASE_H
#define _IMG_BASE_H

double ImgBase_AngleRaise(int *Line);

void ImgBase_WideCaculate();

void ImgBase_GenerateLine();

void ImgBase_ShowDealImg(int x,int y);

void ImgBase_ShowRawImg(int x,int y);

void ImgBase_GeneratePoint();

void ImgBase_FixLine(int startrowk,int endrowk,int startrow,int endrow,int* Line,int Mode);

void ImgBase_FixCorner(int Direction);

uint8 ImgBase_CornerJudeg();

void ImgBase_CornerFactorJudge();

void ImgBase_ConerActions();

uint8 ImgBase_PodaoJudge();

void ImgBase_PodaoAction();

void ImaBase_PodaoFactorJudge();
void ImgBase_LinkPoint(Point Up,Point Down,int* Line);
unsigned short Rule(int x,int y);
#endif
