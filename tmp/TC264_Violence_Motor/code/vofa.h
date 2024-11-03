/*
 * vofa.h
 *
 *  Created on: 2023年12月23日
 *      Author: Roronoa zoro
 */

#ifndef CODE_VOFA_H_
#define CODE_VOFA_H_
#include "zf_common_headfile.h"

#define VOFA_UART_INDEX             (UART_2)                    //蓝牙串口号
#define channel_num                 (23)                          //VOFA发送的通道数目
#define Buff_num                    ((channel_num)+1)           //VOFA缓存数组的大小,附带VOFA的帧尾
typedef union Vofa_Struct
{
    float   float_data;
    uint8   char_table[4];
}Vofa_float;
typedef enum
{
    wireless_uart,
    bluetooth,
    debug_uart,
}uart_device;

typedef struct
{
        uint8   Control_ID;
        uint8   Control_Type;
        float   Control_Value;
}Vofa_Order;
extern float distance;
void Vofa_test(void);
void Vofa_Tx_process(void);
void Vofa_rx_interrupt_handler (void);
void Vofa_uart_init(uint32 Baud,uart_device);
void Wireless_UartRx_process(void);
void Vofa_Rx_process(void);
#endif /* CODE_VOFA_H_ */
