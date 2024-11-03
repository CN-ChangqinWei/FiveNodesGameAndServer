/*
 * pico_gy.h
 *
 *  Created on: 2024年6月25日
 *      Author: cai
 */

#ifndef __PICO_GY_H__
#define __PICO_GY_H__

#include "pico_link_ii.h"
#include "string.h"

#define swj_BLACK 0
#define swj_WHITE 1
#define swj_RED 2
#define swj_GREEN 3
#define swj_BLUE 4
#define swj_PURPLE 5
#define swj_YELLOW 6
#define swj_CYAN 7
#define swj_ORANGE 8

#define swj_point_type1 1 // 小十字 3x3
#define swj_point_type2 2 // 中十字 5x5
#define swj_point_type3 3 // 大十字 7x7
#define swj_point_type4 4 // 小  X  3x3
#define swj_point_type5 5 // 中  X  5x5
#define swj_point_type6 6 // 大  X  7x7
#define swj_point_type7 7 // 全屏十字
#define swj_point_type8 8 // 横向一条直线 y起作用 x不起作用
#define swj_point_type9 9 // 众向一条直线 x起作用 y不起作用

void sendimg(pico_uint8 *image, pico_uint8 width, pico_uint8 height);
void sendimg_A(pico_uint8 *image, pico_uint8 width, pico_uint8 height);
void sendimg_zoom(pico_uint8 *image, pico_uint8 width, pico_uint8 height, pico_uint8 dis_width, pico_uint8 dis_height);
void sendimg_binary(pico_uint8 *image, pico_uint8 width, pico_uint8 height, pico_uint8 otu);
void sendimg_binary_zoom(pico_uint8 *image, pico_uint8 width, pico_uint8 height, pico_uint8 dis_width, pico_uint8 dis_height, pico_uint8 otu);
void sendimg_binary_CHK(pico_uint8 *image, pico_uint8 width, pico_uint8 height, pico_uint8 otu, pico_uint8 chk);
void sendimg_JPEG(pico_uint8 *image, int lon);

void sendline_clear(pico_uint8 color, pico_uint8 width, pico_uint8 height);
void sendline_type(pico_uint8 color, pico_uint8 *buff, pico_uint32 len, pico_uint8 size_lon);
void sendline(pico_uint8 color, pico_uint8 *buff, pico_uint32 len);
void sendline2(pico_uint8 color, pico_uint8 *linex, pico_uint8 *liney, pico_uint32 len);
void sendline_xy(pico_uint8 *line_zx, pico_uint8 *line_yx, pico_uint32 len);
void sendpoint(pico_uint8 color, pico_uint8 x, pico_uint8 y, pico_uint8 type);

void sendimgAndLine(pico_uint8 color, pico_uint8 *buff, pico_uint32 len);
void sendimgAndLine_type(pico_uint8 color, pico_uint8 *buff, pico_uint32 len, pico_uint8 size_lon);
void sendimgAndLine_point(pico_uint8 color, pico_uint8 x, pico_uint8 y, pico_uint8 type);

void sendimg_BinaryAndLine(pico_uint8 *image, pico_uint8 width, pico_uint8 height, pico_uint8 otu);

void put_int32(pico_uint8 name, int dat);
void put_float(pico_uint8 name, float dat);

void swj_stop();

void swj_start();

#define sendline_uint8(color, buff, len) sendline_type(color, buff, len, 1)
#define sendline_int(color, buff, len) sendline_type(color, buff, len, 4)
#define sendline_uint16(color, buff, len) sendline_type(color, buff, len, 2)

#endif
