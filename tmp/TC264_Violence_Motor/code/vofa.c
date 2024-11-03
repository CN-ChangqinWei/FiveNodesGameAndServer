/*
 * vofa.c
 *
 *  Created on: 2023年12月23日
 *      Author: Roronoa zoro
 */


#include "vofa.h"
#include "filter.h"
#include "pid.h"
#define UART_INDEX              (UART_0   )                           // 默认 UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // 默认 115200
#define UART_TX_PIN             (UART2_TX_P10_5  )                           // 默认 UART0_TX_P14_0
#define UART_RX_PIN             (UART2_RX_P10_6  )                           // 默认 UART0_RX_P14_1

uint8 uart_get_data[64];                                                        // 串口接收数据缓冲区
uint8 fifo_get_data[64];                                                        // fifo 输出读出缓冲区

uint8  get_data = 0;                                                            // 接收数据变量
//uint32 fifo_data_count = 0;



Vofa_float Vofa_Tx_buff[Buff_num];          //创建buff_num个发送通道
Vofa_float Vofa_Rx_Data;

Vofa_Order RX_Order;


uint8 data_len;
uint8 Vofa_Rx_buff[64];                     //串口接收数组缓冲区
uint8 Vofa_FIFO_OUT[64];                    //串口缓冲区输出数组

uint8 Vofa_get_data=0;                      //串口中断接收的基本单元
uint32 fifo_data_count = 0;
const uint8 Vofa_test_buff[5]={0x01,0x00,0x00,0x80,0x7f};   //串口初始化时向VOFA发送测试数组
static uint8 Vofa_device;
fifo_struct uart_data_fifo;                 //串口接收缓冲区配置结构体
float distance;
/**
 *@brief       VOFA通道数据设置
 *@para        index设置通道序列号0~channel_num。 Value 发送的数据
 *@return      无
 *@example
 *@Attention:
*/
void Vofa_Set_Channel(uint8 index,float Value)
{
//    zf_log(index<=channel_num,"channel overflow");//通道数超出
//    zf_log(index<1,"channel cannot less than 1");//通道数不可小于1
    Vofa_Tx_buff[index-1].float_data=Value;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       Vofa_uart_init串口硬件初始化
// 参数说明       void
// 返回参数       void
// 使用示例       Vofa_Tx_process();放在定时器中使用，也可以放在主函数中
//-------------------------------------------------------------------------------------------------------------------
void Vofa_uart_init(uint32 Baud,uart_device device)
{
    Vofa_Tx_buff[channel_num].char_table[0]=0x00;
    Vofa_Tx_buff[channel_num].char_table[1]=0x00;
    Vofa_Tx_buff[channel_num].char_table[2]=0x80;
    Vofa_Tx_buff[channel_num].char_table[3]=0x7f;
    switch(device)
    {
        case wireless_uart:wireless_uart_init();
            break;
        case bluetooth:
                        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, Vofa_Rx_buff, 64);   // 初始化 fifo 挂载缓冲区
                        uart_init(UART_2,Baud,UART2_TX_P10_5,UART2_RX_P10_6);                  //串口硬件初始化
                        uart_rx_interrupt(VOFA_UART_INDEX, 1);                               // 开启 UART_INDEX 的接收中断
            break;
        case debug_uart:
                        uart_init(UART_0,Baud,UART0_TX_P14_0,UART0_RX_P14_1);                  //串口硬件初始化
                        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, Vofa_Rx_buff, 64);   // 初始化 fifo 挂载缓冲区
                        uart_rx_interrupt(VOFA_UART_INDEX, 1);                               // 开启 UART_INDEX 的接收中断
                        uart_write_buffer(VOFA_UART_INDEX,Vofa_test_buff,5);                                // 输出测试信息
            break;
    }
    Vofa_device=device;


}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       Vofa_Tx_process，每次发送发送缓冲区中的数据，
// 参数说明       void
// 返回参数       void
// 使用示例       Vofa_Tx_process();放在定时器中使用，也可以放在主函数中
//-------------------------------------------------------------------------------------------------------------------
void Vofa_Tx_process(void)
{
    uint8_t i=1;
    //1-5
    Vofa_Set_Channel(i++,euler_deg.Pitch);
    Vofa_Set_Channel(i++,Motor.duty_L);
    Vofa_Set_Channel(i++,Angle_Target);
    Vofa_Set_Channel(i++,Angle_Gain);
    Vofa_Set_Channel(i++,Motor_Voltage);
    //6-10
    Vofa_Set_Channel(i++,Speed_Target);
    Vofa_Set_Channel(i++,Angle_IN);
    Vofa_Set_Channel(i++,Gyro_loop.P);
    Vofa_Set_Channel(i++,Gyro_loop.I);
    Vofa_Set_Channel(i++,Gyro_loop.D);
    //11-15
    Vofa_Set_Channel(i++,Angle_Field);
    Vofa_Set_Channel(i++,Angle_loop.P);
    Vofa_Set_Channel(i++,Angle_loop.I);
    Vofa_Set_Channel(i++,Angle_loop.D);
    Vofa_Set_Channel(i++,Obstacle);
    //16-20
    Vofa_Set_Channel(i++,Direction_loop.P);
    Vofa_Set_Channel(i++,distance);
    Vofa_Set_Channel(i++,Direction_loop.D);
    Vofa_Set_Channel(i++,Direction_loop.OutPut);
    Vofa_Set_Channel(i++,Dynamic_delt);
    Vofa_Set_Channel(i++,Error);
    Vofa_Set_Channel(i++,Dynamic_Zero);
    //26-30
    Vofa_Set_Channel(i++,openPWM);


    switch(Vofa_device)
    {
        case wireless_uart:wireless_uart_send_buffer(Vofa_Tx_buff[0].char_table,Buff_num*4);
            break;
        case bluetooth:uart_write_buffer(UART_2,Vofa_Tx_buff[0].char_table,Buff_num*4);
            break;
        case debug_uart:uart_write_buffer(UART_0,Vofa_Tx_buff[0].char_table,Buff_num*4);
            break;
    }

}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介       Vofa_Rx_process 每次接收到数据的时候进入接收数据中断，执行此函数
// 参数说明       void
// 返回参数       void
// 使用示例       Vofa_Rx_process();放在接收中断中使用，用于VOFA命令处理
//-------------------------------------------------------------------------------------------------------------------
void Vofa_Rx_process(void)
{
    fifo_data_count=fifo_used(&uart_data_fifo) ;
    if(fifo_data_count != 0)                                                // 读取到数据了
    {
        fifo_read_buffer(&uart_data_fifo, Vofa_FIFO_OUT, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
        /*此处可以调用VOFA_FIFO_OUT中的数据来做串口接收处理*/
        uint8 p=0;
        if(Vofa_FIFO_OUT[0]==0xAA&&Vofa_FIFO_OUT[1]==0xFF)
        {
            RX_Order.Control_Type=Vofa_FIFO_OUT[2];
                        RX_Order.Control_ID=Vofa_FIFO_OUT[3];
                        for(p=0;p<4;p++){Vofa_Rx_Data.char_table[p]=Vofa_FIFO_OUT[p+4];}
                        RX_Order.Control_Value=Vofa_Rx_Data.float_data;
            if(RX_Order.Control_Type==0x00)
            {
                switch(RX_Order.Control_ID)
                {
                    case 0x00:Gyro_loop.P=RX_Order.Control_Value;break;/*Gyro_Kp*/
                    case 0x01:Gyro_loop.I=RX_Order.Control_Value;break;/*Gyro_Ki*/
                    case 0x02:Gyro_loop.D=RX_Order.Control_Value;break; /*Gyro_Kd*/
                    case 0x03:ServoBalance_loop.P=RX_Order.Control_Value;break;/*Gyro_Ilimt*/
                    case 0x04:Angle_Field=RX_Order.Control_Value;break;/*GyroPWMLim*/
                    case 0x05:Angle_Gain=RX_Order.Control_Value;break;
                    case 0x06:Angle_loop.P=RX_Order.Control_Value;break;/*Anlge_Kp*/
                    case 0x07:Angle_loop.I=RX_Order.Control_Value;break;/*Anlge_Ki*/
                    case 0x08:Angle_loop.D=RX_Order.Control_Value;break;/*Anlge_Kd*/
                    case 0x09:Motor_Voltage=RX_Order.Control_Value;break;/*Anlge_Ilimit*/
                    case 0x0A:break;/*Anlge_PWM_L*/
                    case 0x0B:Direction_loop.P=RX_Order.Control_Value;break;
                    case 0x0C:Direction_loop.D=RX_Order.Control_Value;break;
                    case 0x0D:Dynamic_delt=RX_Order.Control_Value;break;
                    case 0x0E:Dynamic_Zero=RX_Order.Control_Value;break;
                    case 0x0F:break;
                    default:break;
                }
            }
            else if(RX_Order.Control_Type==0x01)
            {
                switch(RX_Order.Control_ID)
                {
                    case 0x00:Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=Forward_loop.out_flag=Zgyro_loop.out_flag= RX_Order.Control_Value;Motor.brake=!RX_Order.Control_Value;break;
                    case 0x01:Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag= RX_Order.Control_Value;break;
                    case 0x02:Speed_loop.out_flag=RX_Order.Control_Value;break;
                    case 0x03:ForwardFlag=RX_Order.Control_Value;break;
                    case 0x04:Zgyro_loop.out_flag=Gyro_loop.out_flag=Direction_loop.out_flag=Angle_loop.out_flag=Forward_loop.out_flag= RX_Order.Control_Value;Motor.brake=!RX_Order.Control_Value;break;
                    case 0x05:FFPSign=0;TFPSign=0;break;//设置前后最大值
                    case 0x06:TurnFoundPWM=RX_Order.Control_Value;break;//设置左右最大值
                    case 0x07:FFPSign=RX_Order.Control_Value;break;//设置前后权重
                    case 0x08:TFPSign=RX_Order.Control_Value;break;//设置左右权重
                    case 0x09:Task_ControlMode=(Task_ControlMode?0:1);FFPSign=0;TFPSign=0;break;//切换控制模式
                    case 0x0A:ForwardFoundPWM=RX_Order.Control_Value;break;
                    case 0x0B:TurnFoundPWM=RX_Order.Control_Value;break;
                    case 0x0C:Angle_Field=RX_Order.Control_Value;break;
                    case 0x0D:PID_Param_Zero(RX_Order.Control_Value);break;
                    case 0x0E:PID_Param_Default(RX_Order.Control_Value);break;
                    case 0x0F:Angle_Gain=RX_Order.Control_Value;break;
                    case 0x10:break;
                }

            }

        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介       Vofa_rx_interrupt_handler 的接收中断处理函数 这个函数将在 UART_INDEX 对应的中断调用
// 参数说明       void
// 返回参数       void
// 使用示例       uart_rx_interrupt_handler();
//-------------------------------------------------------------------------------------------------------------------
void Vofa_rx_interrupt_handler (void)
{
    uart_query_byte(VOFA_UART_INDEX, &Vofa_get_data); // 接收数据 查询式 有数据会返回 TRUE 没有数据会返回 FALSE
    fifo_write_buffer(&uart_data_fifo, &Vofa_get_data, 1);                           // 将数据写入 fifo 中


}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介       Vofa_test 串口测试函数，测试模块好坏可以调用此函数
// 参数说明       void
// 返回参数       void
// 使用示例       Vofa_test();
//-------------------------------------------------------------------------------------------------------------------
void Vofa_test(void)
{
    fifo_data_count = fifo_used(&uart_data_fifo);                           // 查看 fifo 是否有数据
    if(fifo_data_count != 0)                                                // 读取到数据了
    {
        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // 将 fifo 中数据读出并清空 fifo 挂载的缓冲
        uart_write_string(UART_2, "\r\nUART get data:");                // 输出测试信息
        uart_write_buffer(UART_2, fifo_get_data, fifo_data_count);      // 将读取到的数据发送出去
    }
    system_delay_ms(10);
}

