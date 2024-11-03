/*
 * lcd.c
 *
 *  Created on: 2023年12月27日
 *      Author: Roronoa zoro
 */
#include "lcd.h"
#define menu_1_num  3

void param_1(void);
void camera_1(void);
void task_1(void);
void param_2(void);
void camera_2(void);
void task_2(void);
Menu_table table[30]={
        //一级菜单
        {0,0,1,0,0,5,(*param_1)},
        {1,0,2,1,1,6,(*camera_1)},
        {2,1,3,2,2,7,(*task_1)},
        //二级菜单
        {5,5,5,5,5,5,(*param_2)},
        {6,6,6,6,6,6,(*camera_2)},
        {7,7,7,7,7,7,(*task_2)},
};

uint8_t  func_index = 0;                    //主程序此时所在程序的索引值
uint8   Index_pos;
uint8   updata;                           //进入显示函数的标志
void (*current_operation_index)(void);      //定义空函数指针

void  Menu_key_set(void)
{
    static uint8 first_show=0;
    //坐标‘>’闪烁
    if(Lcd_index_cnt)ips200_show_string(COL(0),Index_pos,">");
    else ips200_show_string(COL(0),Index_pos," ");
    if(first_show==0)
    {
        uint8 p=0;
        first_show=1;
        ips200_show_string(COL(1),p++,"Param:");
        ips200_show_string(COL(1),p++,"Camera:");
        ips200_show_string(COL(1),p++,"Task:");
    }
    switch(Key.Trig)
    {
        case BUTTON_1:  //如果下一界面序列号不是该序列号，更新标志位置1
                        if(func_index!=table[func_index].on)updata=1;
                        func_index=table[func_index].on;         break;
        case BUTTON_2:  if(func_index!=table[func_index].down)updata=1;
                        func_index=table[func_index].down;       break;
        case BUTTON_3:  if(func_index!=table[func_index].left)updata=1;
                        func_index=table[func_index].left;       break;
        case BUTTON_4:  if(func_index!=table[func_index].right)updata=1;
                        func_index=table[func_index].right;      break;
        case BUTTON_5:  if(func_index!=table[func_index].center)updata=1;
                        func_index=table[func_index].center;     break;
        default:break;
    }
    //如果是一级菜单才更新序列号>
    if(func_index<3)Index_pos=func_index;
    current_operation_index=table[func_index].current_operation;  //执行当前索引号所对应的功能函数
    (*current_operation_index)();//执行当前操作函数

}
//    switch(Key.Trig)
//    {
//        case BUTTON_1:          break;
//        case BUTTON_2:          break;
//        case BUTTON_3:          break;
//        case BUTTON_4:          break;
//        case BUTTON_5:          break;
//        default:break;
//    }
void param_1(void)
{
    //如果从别的菜单进入此菜单更行序列‘>’
    if(updata)
    {
        ips200_clear_region(0,0,8,ROW(menu_1_num));
        uint8 p=0;
        ips200_show_string(DATA_COL,p++,"Zero_Angle");//1.4
        ips200_show_string(DATA_COL,p++,"Speed");
    }

}
void param_2(void)
{

}
void camera_1(void)
{
    if(updata)
    {
        ips200_clear_region(0,0,8,ROW(menu_1_num));
        uint8 p=0;
        ips200_show_string(DATA_COL,p++,"exposure");
        ips200_show_string(DATA_COL,p++,"weight");
    }
}
void camera_2(void)
{

}
void task_1(void)
{

}
void task_2(void)
{

}
void Show_process(void)
{
    uint8 p=0;
    ips200_show_uint(DATA_COL,ROW(p++),Motor.L_dir,1);
    ips200_show_int(DATA_COL,ROW(p++),Motor.duty_L,4);
    ips200_show_uint(DATA_COL,ROW(p++),Motor.R_dir,1);
    ips200_show_int(DATA_COL,ROW(p++),Motor.duty_R,4);
    ips200_show_int(DATA_COL,ROW(p++),Motor.limt,4);
    ips200_show_int(DATA_COL,ROW(p++),Motor.brake,4);
    if(Key.Trig!=0x00)ips200_show_uint(DATA_COL,ROW(6),Key.Trig,4);
    ips200_show_uint(DATA_COL,ROW(7),Key.Long,4);
}
