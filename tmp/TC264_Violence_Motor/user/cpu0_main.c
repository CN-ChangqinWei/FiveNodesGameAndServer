
#include "zf_common_headfile.h"
#include "isr_config.h"
#include "Menu.h"
#include "Img.h"
#include "pico_gy.h"
#include "led.h"
#pragma section all "cpu0_dsram"

    static int Menu_flag=1;
    int last_time=0;
                                                   // fifo 閺佺増宓佹稉顏呮殶
#define IPCH 60 // 娑撳﹣缍呴張鐑樻▔缁�鍝勬禈閸嶅繘鐝惔锟�
#define IPCW 94 // 娑撳﹣缍呴張鐑樻▔缁�鍝勬禈閸嶅繐顔旀惔锟�
#define RED 1
#define BLUE 2
#define YELLOW 3
#define GREEN 4

float Mh = MT9V03X_H;
float Lh = IPCH;
float Mw = MT9V03X_W;
float Lw = IPCW;
uint8 Image_Use[IPCH][IPCW];

void Compressimage()
{
    int i, j, row, line;
    const float div_h = Mh / Lh, div_w = Mw / Lw;
    for (i = 0; i < IPCH; i++)
    {
        row = i * div_h + 0.5;
        for (j = 0; j < IPCW; j++)
        {
            line = j * div_w + 0.5;
            Image_Use[i][j] = mt9v03x_image[row][line];
        }
    }
    // 娴ｈ法鏁ょ�瑰奔绔寸敮顪奙A娴肩姾绶惃鍕禈閸嶅繐娴橀崓锟�  閸欘垯浜掑锟芥慨瀣╃炊鏉堟挷绗呮稉锟界敮锟�
}
int core0_main(void)
{
    clock_init();                   // 閼惧嘲褰囬弮鍫曟寭妫版垹宸�<閸斺�崇箑娣囨繄鏆�>
    debug_init();                   // 閸掓繂顫愰崠鏍帛鐠併倛鐨熺拠鏇氳閸欙拷
    // 濮濄倕顦╃紓鏍у晸閻€劍鍩涙禒锝囩垳 娓氬顩ф径鏍啎閸掓繂顫愰崠鏍﹀敩閻胶鐡�
    Img_Init();
    //Menu_Init();
    Device_Init();
    Algorithms_Init();
    dot_matrix_screen_init();
//    int16 TT=0;
//    uint8 dir_T=0;
//    Motor.brake=0;
    // 濮濄倕顦╃紓鏍у晸閻€劍鍩涙禒锝囩垳 娓氬顩ф径鏍啎閸掓繂顫愰崠鏍﹀敩閻胶鐡�
    ips200_show_float(0,0,Vol.Bend,3,1);
    ips200_show_float(40,0,Vol.Cross,3,1);
    ips200_show_float(80,0,Vol.Island,3,1);
    ips200_show_float(120,0,Vol.Normal,3,1);
    ips200_show_int(0,0,Keys[0],3);
    ips200_show_int(40,0,Keys[1],3);
    ips200_show_int(80,0,Keys[2],3);
    ips200_show_int(120,0,Keys[3],3);
    cpu_wait_event_ready();         // 缁涘绶熼幍锟介張澶嬬壋韫囧啫鍨垫慨瀣鐎瑰本鐦�

    while (TRUE)
    {

        Compressimage();                          // 閸樺缂夐崶鎯у剼
        sendline_clear(swj_BLACK,94,60);
        sendline_type(swj_RED,(pico_uint8 *)LeftLine,60,4);
        sendline_type(swj_YELLOW,(pico_uint8 *)RightLine,60,4);
        sendline_type(swj_GREEN,(pico_uint8 *)CenterLine,60,4);
        sendimg((pico_uint8 *)Image_Use, 94, 60); // 閸欐垿锟戒礁娴橀崓锟�
    }
}
//        //濞村鐦紓鏍垳閸ｏ拷
//        if(TT>3000)
//        {
//            dir_T=1;
//        }
//        if(TT<-3000)
//        {
//            dir_T=0;
//        }
//        if(dir_T==0)TT+=10;
//        else if(dir_T==1)TT-=10;
//        Motor.duty_L=TT;
//        Motor.duty_R=TT;
//        Set_PWM(&Motor);
//        system_delay_ms(10);


#pragma section all restore
