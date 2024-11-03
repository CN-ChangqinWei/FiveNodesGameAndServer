/*
 * led_loop.c
 *
 *  Created on: 2024Äê4ÔÂ27ÈÕ
 *      Author: DELL
 */
#include "zf_common_headfile.h"
#include "zf_device_tld7002.h"
#include "zf_device_dot_matrix_screen.h"
#include "led.h"
enum Dir direction;
int nCross=0;
enum Ele element;
void led_loop(void)
{        dot_matrix_screen_set_brightness(5000);
    int d,e;
    e=element;
    d=direction;
    //dot_matrix_screen_show_string("OK");
    static int crossflag=0;
    char text[3]={0};
    if(Motor.brake==0)
    {
        switch(d)
            {
                case 0:dot_matrix_screen_show_string_single(2,"!");break;
                case 1:dot_matrix_screen_show_string_single(2,"#");break;
                case 2:dot_matrix_screen_show_string_single(2,"<");break;
                case 3:dot_matrix_screen_show_string_single(2,">");break;
            }
           if(Motor.brake==0&&ForwardFlag==0){
               dot_matrix_screen_show_string("OK");
           }else{

               if(e==cross&&crossflag==0){
                   nCross++;
                   crossflag=1;

               }else if(e!=cross){
                   crossflag=0;

               }
               sprintf(text,"A%d",nCross%10);
               dot_matrix_screen_show_string(text);
           }
    }
    else {
        dot_matrix_screen_show_string("NG");


    }


}


