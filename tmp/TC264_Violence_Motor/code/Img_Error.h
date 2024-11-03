
#include "Img_Config.h"
#ifndef _IMG_ERROR_H
#define _IMG_ERROR_H
extern double Error_dot, Error_dot_dot;
extern double B,A;
extern double Error;
void ImgError_GernerateErr(int* Line,int startline,int endline);
double ImgError_CaculateK(int*Line,int startline,int endline );
#endif
