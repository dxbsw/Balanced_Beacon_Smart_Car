/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技F3277核心板和母板
【编    写】龙邱科技
【E-mail  】chiusir@163.com
【软件版本】V1.0 版权所有，单位使用请先联系授权
【最后更新】2020年12月24日，持续更新，请关注最新版！
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】IAR7.8 KEIL5.24及以上版本
【Target 】 MM32F3277
【SYS PLL】 120MHz 频率太高可能无法启动system_mm32f327x.c
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_TIMER_H_
#define __LQ_TIMER_H_
#include "include.h"
extern float BalDirgyro_out;
extern float BALANCE_OUT;
extern float BalDir_out;
extern float SPEED_OUT;
typedef enum
{
  TIMER_1,//摄像头用
  TIMER_2,//编码器用2
  TIMER_3,//编码器用3	
  TIMER_4,//编码器用4
  TIMER_5,//舵机用	
  TIMER_6,//定时器用
  TIMER_7,//定时器用,pid等
  TIMER_8,//电机用
}TIMER_Name_t;
extern const u32 TIMERx[] ;
extern TIM_TypeDef *TIMERxP[8];
void TIM1_Int_Init(u16 x100us);
void TIMER_InitConfig(TIMER_Name_t timern, u16 nms);
void Test_TimerPIT56(void);

void kongzhi_1(void);//龙邱
void kongzhi_2(void);
#endif
