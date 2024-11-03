/*
 * lcd.h
 *
 *  Created on: 2023��12��27��
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
    uint8 current;      //��ǰ״̬������
    uint8 on;           //��
    uint8 down;         //��
    uint8 left;         //��
    uint8 right;        //��
    uint8 center;       //����
    void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
} Menu_table;
void Show_process(void);
#endif /* CODE_LCD_H_ */
