#include "zf_common_headfile.h"
#include "isr_config.h"
#include "isr.h"
#include "Menu.h"
#include "Img.h"
#define _FUNC_ISR_LIMIT(value,min,max)  ((value)>(max)?(max):((value)<(min)?(min):(value)))
#define _DEF_DOTMAX (5)
int16 duty;
uint16 CCU0_CH0_2ms=0,LED1_BLINK_CNT,LED2_BLINK_CNT,CCU0_CH1_50ms,CCU0_CH0_10ms,CCU0_CH0_50ms,CCU0_CH0_2s,CCU0_CH0_200ms=0,CCU0_CH1_10ms=0,CCU0_CH1_1s=0,CCU0_CH1_2s=0,CCU0_CH1_100ms=0;
uint8 Lcd_index_cnt;
// **************************** PIT娑擃厽鏌囬崙鑺ユ殶 ****************************
IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    pit_clear_flag(CCU60_CH0);
    CCU0_CH0_2ms++;CCU0_CH0_10ms++;CCU0_CH0_50ms++;CCU0_CH0_200ms++;CCU0_CH1_1s++;
    if(Lcd_index_cnt)CCU0_CH1_2s++;
    else CCU0_CH1_2s=0;
    static double Error_Last=0;
    //2ms閺冨爼妫块悧锟�
    if(CCU0_CH0_2ms>=2)
    {
        CCU0_CH0_2ms=0;
        IMU_get_euler();                //閼惧嘲褰囧▎褎濯虹憴锟�
        Gyro_2ms();                 //鐟欐帡锟界喎瀹抽悳锟�
    }
    //10ms閺冨爼妫块悧锟�
    if(CCU0_CH0_10ms>=10)
    {
        CCU0_CH0_10ms=0;
        Error_dot=Error-Error_Last;
        if(_FUNC_ABS(Error_dot)>=6) Error=Error_Last+_FUNC_ISR_LIMIT(0.5*Error_dot,-_DEF_DOTMAX,_DEF_DOTMAX);
        Error_Last=Error;
        if(ForwardFlag)
        Direction_10ms();
        Angle_10ms();               //鐟欐帒瀹抽悳锟�
        //if(ForwardFlag)Obstacle_process();
    }
    //50ms閺冨爼妫块悧锟�
    if(CCU0_CH0_50ms>=50)
    {
        CCU0_CH0_50ms=0;
        if(RoadStat!=_DEF_IMG_PODAO){
            Servo_50ms();
        Servo_SetPWM(Mid_PWM-(int)ServoBalance_loop.OutPut);
        }else if(ForwardFlag) Servo_SetPWM(Mid_PWM);
    }
    if(CCU0_CH1_1s>=100)
    {
        CCU0_CH1_1s=0;
//        VBAT=adc_mean_filter_convert(ADC0_CH8_A8,20)*0.01+0.99*VBAT_Last;
        VBAT=adc_mean_filter_convert(ADC0_CH8_A8,50)*1.0f/4096.0f*69.7f-0.04f;
        VBAT_New=VBAT*0.05+VBAT_Last*0.95;
        if(FastFlag==0)
        openPWM=Motor_Voltage/VBAT_New*10000.0f;
        VBAT_Last=VBAT_New;
    }
    if(CCU0_CH1_2s>=2000)
    {
        CCU0_CH1_2s=0;
        Lcd_index_cnt=0;
    }


}

//50ms
IFX_INTERRUPT(cc60_pit_ch1_isr, 0, CCU6_0_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    pit_clear_flag(CCU60_CH1);
    CCU0_CH1_50ms++;
    CCU0_CH1_100ms++;
    //CCU0_CH1_1s++;
    Key_read(&Key);             //鐠囪褰囬幐澶愭暛
//    if(Key.Trig){
//        Menu_Action=Key.Trig;
//        Menu_Proccess();
//    }
    Key_process();              //閹稿鏁径鍕倞
    //100ms閺冨爼妫块悧锟�
    if(CCU0_CH1_50ms>=10)//50ms
    {
        CCU0_CH1_50ms=0;
//        if(Motor.brake==0)
//        {
        Vofa_Tx_process();
//        }

        Vofa_Rx_process();
    }
    if(CCU0_CH1_100ms>=10){
        CCU0_CH1_100ms=0;
        StartTime++;
    }
}

IFX_INTERRUPT(cc61_pit_ch0_isr, 0, CCU6_1_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    pit_clear_flag(CCU61_CH0);




}

IFX_INTERRUPT(cc61_pit_ch1_isr, 0, CCU6_1_CH1_ISR_PRIORITY)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    pit_clear_flag(CCU61_CH1);





}
// **************************** PIT娑擃厽鏌囬崙鑺ユ殶 ****************************


// **************************** 婢舵牠鍎存稉顓熸焽閸戣姤鏆� ****************************
IFX_INTERRUPT(exti_ch0_ch4_isr, 0, EXTI_CH0_CH4_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    if(exti_flag_get(ERU_CH0_REQ0_P15_4))           // 闁岸浜�0娑擃厽鏌�
    {
        exti_flag_clear(ERU_CH0_REQ0_P15_4);
//        wireless_module_uart_handler();                 // 閺冪姷鍤庡Ο鈥虫健缂佺喍绔撮崶鐐剁殶閸戣姤鏆�



    }

    if(exti_flag_get(ERU_CH4_REQ13_P15_5))          // 闁岸浜�4娑擃厽鏌�
    {
        exti_flag_clear(ERU_CH4_REQ13_P15_5);
        //My_l_speed_update();



    }
}

IFX_INTERRUPT(exti_ch1_ch5_isr, 0, EXTI_CH1_CH5_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    if(exti_flag_get(ERU_CH1_REQ10_P14_3))          // 闁岸浜�1娑擃厽鏌�
    {
        exti_flag_clear(ERU_CH1_REQ10_P14_3);



    }

    if(exti_flag_get(ERU_CH5_REQ1_P15_8))           // 闁岸浜�5娑擃厽鏌�
    {

        exti_flag_clear(ERU_CH5_REQ1_P15_8);
        //My_r_speed_update();



    }
}

// 閻㈠彉绨幗鍕剼婢剁clk瀵洝鍓兼妯款吇閸楃姷鏁ゆ禍锟� 2闁岸浜鹃敍宀�鏁ゆ禍搴ば曢崣鎱廙A閿涘苯娲滃銈堢箹闁插奔绗夐崘宥呯暰娑斿鑵戦弬顓炲毐閺侊拷
// IFX_INTERRUPT(exti_ch2_ch6_isr, 0, EXTI_CH2_CH6_INT_PRIO)
// {
//  interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
//  if(exti_flag_get(ERU_CH2_REQ7_P00_4))           // 闁岸浜�2娑擃厽鏌�
//  {
//      exti_flag_clear(ERU_CH2_REQ7_P00_4);
//  }
//  if(exti_flag_get(ERU_CH6_REQ9_P20_0))           // 闁岸浜�6娑擃厽鏌�
//  {
//
//      exti_flag_clear(ERU_CH6_REQ9_P20_0);
//  }
// }

IFX_INTERRUPT(exti_ch3_ch7_isr, 0, EXTI_CH3_CH7_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    if(exti_flag_get(ERU_CH3_REQ6_P02_0))           // 闁岸浜�3娑擃厽鏌�
    {
        exti_flag_clear(ERU_CH3_REQ6_P02_0);
        camera_vsync_handler();                     // 閹藉嫬鍎氭径纾嬓曢崣鎴﹀櫚闂嗗棛绮烘稉锟介崶鐐剁殶閸戣姤鏆�
    }
    if(exti_flag_get(ERU_CH7_REQ16_P15_1))          // 闁岸浜�7娑擃厽鏌�
    {
        exti_flag_clear(ERU_CH7_REQ16_P15_1);
        dot_matrix_screen_scan();


    }
}
// **************************** 婢舵牠鍎存稉顓熸焽閸戣姤鏆� ****************************


// **************************** DMA娑擃厽鏌囬崙鑺ユ殶 ****************************
IFX_INTERRUPT(dma_ch5_isr, 0, DMA_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    camera_dma_handler();                           // 閹藉嫬鍎氭径鎾櫚闂嗗棗鐣幋鎰埠娑擄拷閸ョ偠鐨熼崙鑺ユ殶
}
// **************************** DMA娑擃厽鏌囬崙鑺ユ殶 ****************************


// **************************** 娑撴彃褰涙稉顓熸焽閸戣姤鏆� ****************************
IFX_INTERRUPT(uart0_tx_isr, 0, UART0_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrTransmit(&uart0_handle);


}
IFX_INTERRUPT(uart0_rx_isr, 0, UART0_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrReceive(&uart0_handle);


#if DEBUG_UART_USE_INTERRUPT                        // 婵″倹鐏夊锟介崥锟� debug 娑撴彃褰涙稉顓熸焽
        debug_interrupr_handler();                  // 鐠嬪啰鏁� debug 娑撴彃褰涢幒銉︽暪婢跺嫮鎮婇崙鑺ユ殶 閺佺増宓佹导姘愁潶 debug 閻滎垰鑸扮紓鎾冲暱閸栭缚顕伴崣锟�
#endif                                              // 婵″倹鐏夋穱顔芥暭娴滐拷 DEBUG_UART_INDEX 闁綀绻栧▓鍏稿敩閻線娓剁憰浣规杹閸掓澘顕惔鏃傛畱娑撴彃褰涙稉顓熸焽閸橈拷

}
IFX_INTERRUPT(uart0_er_isr, 0, UART0_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrError(&uart0_handle);



}

// 娑撴彃褰�1姒涙顓绘潻鐐村复閸掔増鎲氶崓蹇撱仈闁板秶鐤嗘稉鎻掑經
IFX_INTERRUPT(uart1_tx_isr, 0, UART1_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrTransmit(&uart1_handle);




}
IFX_INTERRUPT(uart1_rx_isr, 0, UART1_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrReceive(&uart1_handle);
    camera_uart_handler();                          // 閹藉嫬鍎氭径鏉戝棘閺佷即鍘ょ純顔剧埠娑擄拷閸ョ偠鐨熼崙鑺ユ殶
    tld7002_callback();
}
IFX_INTERRUPT(uart1_er_isr, 0, UART1_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrError(&uart1_handle);




}


// 娑撴彃褰�2姒涙顓绘潻鐐村复閸掔増妫ょ痪鑳祮娑撴彃褰涘Ο鈥虫健
IFX_INTERRUPT(uart2_tx_isr, 0, UART2_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrTransmit(&uart2_handle);





}

IFX_INTERRUPT(uart2_rx_isr, 0, UART2_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    Vofa_rx_interrupt_handler();
}

IFX_INTERRUPT(uart2_er_isr, 0, UART2_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrError(&uart2_handle);

}



IFX_INTERRUPT(uart3_tx_isr, 0, UART3_TX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrTransmit(&uart3_handle);




}
IFX_INTERRUPT(uart3_rx_isr, 0, UART3_RX_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷






}
IFX_INTERRUPT(uart3_er_isr, 0, UART3_ER_INT_PRIO)
{
    interrupt_global_enable(0);                     // 瀵拷閸氼垯鑵戦弬顓炵サ婵傦拷
    IfxAsclin_Asc_isrError(&uart3_handle);





}
// **************************** 娑撴彃褰涙稉顓熸焽閸戣姤鏆� ****************************
