/*
 * vofa.c
 *
 *  Created on: 2023��12��23��
 *      Author: Roronoa zoro
 */


#include "vofa.h"
#include "filter.h"
#include "pid.h"
#define UART_INDEX              (UART_0   )                           // Ĭ�� UART_0
#define UART_BAUDRATE           (DEBUG_UART_BAUDRATE)                           // Ĭ�� 115200
#define UART_TX_PIN             (UART2_TX_P10_5  )                           // Ĭ�� UART0_TX_P14_0
#define UART_RX_PIN             (UART2_RX_P10_6  )                           // Ĭ�� UART0_RX_P14_1

uint8 uart_get_data[64];                                                        // ���ڽ������ݻ�����
uint8 fifo_get_data[64];                                                        // fifo �������������

uint8  get_data = 0;                                                            // �������ݱ���
//uint32 fifo_data_count = 0;



Vofa_float Vofa_Tx_buff[Buff_num];          //����buff_num������ͨ��
Vofa_float Vofa_Rx_Data;

Vofa_Order RX_Order;


uint8 data_len;
uint8 Vofa_Rx_buff[64];                     //���ڽ������黺����
uint8 Vofa_FIFO_OUT[64];                    //���ڻ������������

uint8 Vofa_get_data=0;                      //�����жϽ��յĻ�����Ԫ
uint32 fifo_data_count = 0;
const uint8 Vofa_test_buff[5]={0x01,0x00,0x00,0x80,0x7f};   //���ڳ�ʼ��ʱ��VOFA���Ͳ�������
static uint8 Vofa_device;
fifo_struct uart_data_fifo;                 //���ڽ��ջ��������ýṹ��
float distance;
/**
 *@brief       VOFAͨ����������
 *@para        index����ͨ�����к�0~channel_num�� Value ���͵�����
 *@return      ��
 *@example
 *@Attention:
*/
void Vofa_Set_Channel(uint8 index,float Value)
{
//    zf_log(index<=channel_num,"channel overflow");//ͨ��������
//    zf_log(index<1,"channel cannot less than 1");//ͨ��������С��1
    Vofa_Tx_buff[index-1].float_data=Value;
}
//-------------------------------------------------------------------------------------------------------------------
// �������       Vofa_uart_init����Ӳ����ʼ��
// ����˵��       void
// ���ز���       void
// ʹ��ʾ��       Vofa_Tx_process();���ڶ�ʱ����ʹ�ã�Ҳ���Է�����������
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
                        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, Vofa_Rx_buff, 64);   // ��ʼ�� fifo ���ػ�����
                        uart_init(UART_2,Baud,UART2_TX_P10_5,UART2_RX_P10_6);                  //����Ӳ����ʼ��
                        uart_rx_interrupt(VOFA_UART_INDEX, 1);                               // ���� UART_INDEX �Ľ����ж�
            break;
        case debug_uart:
                        uart_init(UART_0,Baud,UART0_TX_P14_0,UART0_RX_P14_1);                  //����Ӳ����ʼ��
                        fifo_init(&uart_data_fifo, FIFO_DATA_8BIT, Vofa_Rx_buff, 64);   // ��ʼ�� fifo ���ػ�����
                        uart_rx_interrupt(VOFA_UART_INDEX, 1);                               // ���� UART_INDEX �Ľ����ж�
                        uart_write_buffer(VOFA_UART_INDEX,Vofa_test_buff,5);                                // ���������Ϣ
            break;
    }
    Vofa_device=device;


}
//-------------------------------------------------------------------------------------------------------------------
// �������       Vofa_Tx_process��ÿ�η��ͷ��ͻ������е����ݣ�
// ����˵��       void
// ���ز���       void
// ʹ��ʾ��       Vofa_Tx_process();���ڶ�ʱ����ʹ�ã�Ҳ���Է�����������
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
// �������       Vofa_Rx_process ÿ�ν��յ����ݵ�ʱ�������������жϣ�ִ�д˺���
// ����˵��       void
// ���ز���       void
// ʹ��ʾ��       Vofa_Rx_process();���ڽ����ж���ʹ�ã�����VOFA�����
//-------------------------------------------------------------------------------------------------------------------
void Vofa_Rx_process(void)
{
    fifo_data_count=fifo_used(&uart_data_fifo) ;
    if(fifo_data_count != 0)                                                // ��ȡ��������
    {
        fifo_read_buffer(&uart_data_fifo, Vofa_FIFO_OUT, &fifo_data_count, FIFO_READ_AND_CLEAN);    // �� fifo �����ݶ�������� fifo ���صĻ���
        /*�˴����Ե���VOFA_FIFO_OUT�е������������ڽ��մ���*/
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
                    case 0x05:FFPSign=0;TFPSign=0;break;//����ǰ�����ֵ
                    case 0x06:TurnFoundPWM=RX_Order.Control_Value;break;//�����������ֵ
                    case 0x07:FFPSign=RX_Order.Control_Value;break;//����ǰ��Ȩ��
                    case 0x08:TFPSign=RX_Order.Control_Value;break;//��������Ȩ��
                    case 0x09:Task_ControlMode=(Task_ControlMode?0:1);FFPSign=0;TFPSign=0;break;//�л�����ģʽ
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
// �������       Vofa_rx_interrupt_handler �Ľ����жϴ����� ����������� UART_INDEX ��Ӧ���жϵ���
// ����˵��       void
// ���ز���       void
// ʹ��ʾ��       uart_rx_interrupt_handler();
//-------------------------------------------------------------------------------------------------------------------
void Vofa_rx_interrupt_handler (void)
{
    uart_query_byte(VOFA_UART_INDEX, &Vofa_get_data); // �������� ��ѯʽ �����ݻ᷵�� TRUE û�����ݻ᷵�� FALSE
    fifo_write_buffer(&uart_data_fifo, &Vofa_get_data, 1);                           // ������д�� fifo ��


}

//-------------------------------------------------------------------------------------------------------------------
// �������       Vofa_test ���ڲ��Ժ���������ģ��û����Ե��ô˺���
// ����˵��       void
// ���ز���       void
// ʹ��ʾ��       Vofa_test();
//-------------------------------------------------------------------------------------------------------------------
void Vofa_test(void)
{
    fifo_data_count = fifo_used(&uart_data_fifo);                           // �鿴 fifo �Ƿ�������
    if(fifo_data_count != 0)                                                // ��ȡ��������
    {
        fifo_read_buffer(&uart_data_fifo, fifo_get_data, &fifo_data_count, FIFO_READ_AND_CLEAN);    // �� fifo �����ݶ�������� fifo ���صĻ���
        uart_write_string(UART_2, "\r\nUART get data:");                // ���������Ϣ
        uart_write_buffer(UART_2, fifo_get_data, fifo_data_count);      // ����ȡ�������ݷ��ͳ�ȥ
    }
    system_delay_ms(10);
}

