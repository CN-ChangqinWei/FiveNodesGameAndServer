/*
 * lcd.h
 *
 *  Created on: 2023年12月27日
 *      Author: Roronoa zoro
 */

#ifndef CODE_LCD_H_
#define CODE_LCD_H_
#include "zf_common_headfile.h"
#define COL(x)          (x*8)
#define ROW(y)          (y*16)
#define IPS_Y_MAX       (320)
#define IPS_X_MAX       (240)
#define DATA_COL        (124)

#define MiddleLine      (120)

typedef struct
{
    uint8 current;      //当前状态索引号
    uint8 on;           //上
    uint8 down;         //下
    uint8 left;         //左
    uint8 right;        //右
    uint8 center;       //中心
    void (*current_operation)(void); //当前状态应该执行的操作
} Menu_table;
void Show_process(void);
#endif /* CODE_LCD_H_ */
