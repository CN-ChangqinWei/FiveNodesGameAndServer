#include"Img.h"
#include "pico_gy.h"
int RoadStat=0;
int FactorStat=0;
int ImgShowFlag=0;
int Error_ZeroFlag=0;
int UpStart=0;
int ErrUp=25;
int ErrDown=59;
int StartTime=0;
double Error_FactorProvide=0;
IMG_FUNC ImgActions[_DEF_IMG_NOFFACTORS]={0};
int Width[60]={0};
int Img_ExposureTime=180;
int Img_RingFlag=1;
int Img_S=0;
void Img_Init(){

    while(mt9v03x_init());
    mt9v03x_set_exposure_time(Img_ExposureTime);
    ips200_set_dir(IPS200_PORTAIT);
    ips200_init(IPS200_TYPE_PARALLEL8);
    RectInit();
    ImgActions[_DEF_IMG_CROSS]=ImgCross_Action;
    ImgActions[_DEF_IMG_NORMAL]=Img_NormalAction;
    ImgActions[_DEF_IMG_CORNER]=ImgBase_ConerActions;
    ImgActions[_DEF_IMG_ISLANDLEFT]=ImgIsland_Action;
    ImgActions[_DEF_IMG_ISLANDRIGHT]=ImgIsland_Action;
    ImgActions[_DEF_IMG_PARKLEFT]=ImgPark_Action;
    ImgActions[_DEF_IMG_PARKRIGHT]=ImgPark_Action;
    ImgActions[_DEF_IMG_PODAO]=ImgBase_PodaoAction;
    ImgActions[_DEF_IMG_STOP]=ImgPark_Action;
    ImgCross_Init();
    ImgIsland_Init();
}
void Img_ShowReflect(uint16 x, uint16 y,const uint8** map, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 threshold)
{


    uint32 i = 0, j = 0;
        uint16 color = 0;
        uint8 temp = 0;
        uint32 width_index = 0, height_index = 0;

        ips200_set_region(x, y, x + dis_width - 1, y + dis_height - 1);             // 设置显示区域

        for(j = 0; j < dis_height; j ++)
        {
            height_index = j * height / dis_height;
            for(i = 0; i < dis_width; i ++)
            {
                width_index = i * width / dis_width;
                temp = *(*(map + height_index * width + width_index));               // 读取像素点
                if(threshold == 0)
                {
                    color = (0x001f & ((temp) >> 3)) << 11;
                    color = color | (((0x003f) & ((temp) >> 2)) << 5);
                    color = color | (0x001f & ((temp) >> 3));
                    ips200_write_16bit_data(color);
                }
                else if(temp < threshold)
                {
                    ips200_write_16bit_data(RGB565_BLACK);
                }
                else
                {
                    ips200_write_16bit_data(RGB565_WHITE);
                }
            }
        }
}

void Img_Proccess(){
    if(mt9v03x_finish_flag){

    ImgBase_GenerateLine();
    Img_JudgeFactor();
    for(int i=0;i<60;i++){
        LeftLinePre[i]=LeftLine[i];
        RightLinePre[i]=RightLine[i];
        if(!_FUNC_IMG_OUTX(CenterLine[i]))
                        {

                        Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
                        }
    }
    if(_DEF_IMG_PODAO==RoadStat){
        ImgError_GernerateErr(CenterLine,15,59);
    }
    else
    ImgError_GernerateErr(CenterLine,10,50);
    //ImgError_GernerateErr(CenterLine,ErrUp,ErrDown);
    if(ImgShowFlag&&Motor.brake){
                    Img_ShowReflect(0,0,ReflactMap, _DEF_IMG_WIDTH, _DEF_IMG_HEIGHT, _DEF_IMG_WIDTH*2,_DEF_IMG_HEIGHT*2, 0);
                    unsigned short temp;
                    for(int i=0;i<_DEF_IMG_WIDTH;i++){
                        for(int j=0;j<_DEF_IMG_HEIGHT;j++){
                            if(Img_VisitFlag[j][i]){

                                ips200_draw_point ( i*2, j*2, Img_VisitFlag[j][i]);
                                ips200_draw_point ( i*2+1, j*2, Img_VisitFlag[j][i]);
                                ips200_draw_point ( i*2, j*2+1, Img_VisitFlag[j][i]);
                                ips200_draw_point ( i*2+1, j*2+1, Img_VisitFlag[j][i]);
                            }
                        }
                    }
                    ips200_draw_point ( 46*2, _DEF_IMG_RECENT*2, _DEF_IMG_POINTCOLOR);
                    ips200_draw_point ( 46*2+1, _DEF_IMG_RECENT*2, _DEF_IMG_POINTCOLOR);
                    ips200_draw_point ( 46*2, _DEF_IMG_RECENT*2+1, _DEF_IMG_POINTCOLOR);
                    ips200_draw_point ( 46*2+1, _DEF_IMG_RECENT*2+1, _DEF_IMG_POINTCOLOR);
                    ips200_show_float(0,_DEF_IMG_HEIGHT*4,Error,3,3);
                    ips200_show_int(0,_DEF_IMG_HEIGHT*4+40,Img_S,5);
                    ips200_show_int(60,_DEF_IMG_HEIGHT*4+40,JumpPoint,5);
                    ips200_show_int(120,_DEF_IMG_HEIGHT*4+40,RightLoseNum,5);
                    ips200_show_int(0,_DEF_IMG_HEIGHT*4+60,RoadStat,5);
                    ips200_show_int(30,_DEF_IMG_HEIGHT*4+60,FactorStat,5);
                    ips200_show_int(60,_DEF_IMG_HEIGHT*4+60,EndLine,5);
                    ips200_show_int(90,_DEF_IMG_HEIGHT*4+60,LeftLoseNumRecent,3);
                    ips200_show_int(120,_DEF_IMG_HEIGHT*4+60,RightLoseNumRecent,3);
                    ips200_show_int(150,_DEF_IMG_HEIGHT*4+60,LeftNodeNum,3);
                    ips200_show_int(180,_DEF_IMG_HEIGHT*4+60,RightNodeNum,3);
                }else if(ImgShowFlag&&Motor.brake==0) {ips200_show_int(0,_DEF_IMG_HEIGHT*4+60,RoadStat,5);
                ips200_show_int(30,_DEF_IMG_HEIGHT*4+60,FactorStat,5);}

               mt9v03x_finish_flag=0;


            }
}

void Img_JudgeFactor(){
    if(ImgActions[RoadStat]){
        (*ImgActions[RoadStat])();
    }
}

void Img_NormalAction(){
    //CalWidthFlag=0;
    CalWidthFlag=1;
    Error_ZeroFlag=0;
    if(ImgPark_RoadJudge()) return;
    if(ImgBase_PodaoJudge()) return;
    if(ImgCross_RoadJudge()) return;
    if(Img_RingFlag)
        if(ImgIsland_RoadJudge()) return;


    int temp;
    //if(ImgBase_CornerJudeg()) return;
    if(LeftLoseNum>RightLoseNum){
        ImgBase_FixCorner(_DEF_IMG_LEFT);
    }
    else{

        ImgBase_FixCorner(_DEF_IMG_RIGHT);
    }

    element=normal;
//    for(int i=_DEF_IMG_SEARCHBEGIN;i>_DEF_IMG_SEARCHEND;i--){
//                if(Img_VisitFlag[i][_DEF_IMG_LEFTLIMIT+1]&&RightLine[i]){
//                    temp=RightLine[i]-(LineWidth[i])*_DEF_IMG_CORNERK;
//                    if(!_FUNC_IMG_OUTX(temp))
//                    {
//                        CenterLine[i]=temp;
//                    Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
//                    }
//                    else{
//                                       CenterLine[i]=temp;
//                         }
//                }
//
//            }
//    for(int i=_DEF_IMG_SEARCHBEGIN;i>_DEF_IMG_SEARCHEND;i--){
//
//                    //if(LeftLine[i]&&Img_VisitFlag[i][_DEF_IMG_RIGHTLIMIT-1]){
//                        temp=LeftLine[i]+(LineWidth[i])*_DEF_IMG_CORNERK;
//                        if(!_FUNC_IMG_OUTX(temp)){
//
//                            CenterLine[i]=temp;
//                        Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
//
//                        }
//                        else{
//
//                            CenterLine[i]=temp;
//                            //Img_VisitFlag[i][CenterLine[i]]=_DEF_IMG_CENTERLINE;
//                        }
//                    //}
//
//                }

}


