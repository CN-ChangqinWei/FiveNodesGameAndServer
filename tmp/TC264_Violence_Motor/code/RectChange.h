/*
 * RectChange.h
 *
 *  Created on: 2023Äê11ÔÂ21ÈÕ
 *      Author: howlo
 */
#include "zf_common_headfile.h"
//#include "camera.h"
#ifndef CODE_RECTCHANGE_H_
#define CODE_RECTCHANGE_H_

typedef struct Loca
{
        int x;
        int y;


}TypedefLoca,*ptrLoca;

extern uint8* ReflactMap[60][94];
void RectChanging(double (*rect)[3]);
void RectCal(float (*RectL)[3],float (*RectR)[3],float (*RectResult)[3]);
void RectChanging_QICI(double (*rect)[3]);
double AngleToTheOne(double angle);
#endif /* CODE_RECTCHANGE_H_ */
