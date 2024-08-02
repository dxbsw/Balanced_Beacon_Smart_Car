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
#ifndef __LQ_ENC_H_
#define __LQ_ENC_H_
#include "include.h"


typedef enum
{
  //定时器BIT15-12  复用BIT11-8 分组BIT7-4  管脚BIT3-0
  
  TIM1_ENCA_A8 = 0x0108,// AF1 
  TIM1_ENCA_E9 = 0x0149,// AF1 
  TIM1_ENCB_A9 = 0x0109,// AF1 
  TIM1_ENCB_E11= 0x014B,// AF1 
    
  TIM2_ENCA_A0 = 0x1100,// AF1 
  TIM2_ENCA_A5 = 0x1105,// AF1 
  TIM2_ENCA_A15= 0x110F,// AF1  
  TIM2_ENCB_A1 = 0x1101,// AF1 
  TIM2_ENCB_B3 = 0x1113,// AF1 
  
  TIM3_ENCA_A6 = 0x2206,// AF2 
  TIM3_ENCA_B4 = 0x2214,// AF2 
  TIM3_ENCA_C6 = 0x2226,// AF2   
  TIM3_ENCB_A7 = 0x2207,// AF2 
  TIM3_ENCB_B5 = 0x2215,// AF2 
  TIM3_ENCB_C7 = 0x2227,// AF2 
 
  TIM4_ENCA_B6 = 0x3216,// AF2 
  TIM4_ENCA_D12= 0x323C,// AF2   
  TIM4_ENCB_B7 = 0x3217,// AF2 
  TIM4_ENCB_D13= 0x323D,// AF2 
  
  TIM5_ENCA_A0 = 0x4200,// AF2 
  TIM5_ENCB_A1 = 0x4201,// AF2 
  
  TIM8_ENCA_C6 = 0x7326,// AF3 
  TIM8_ENCB_C7 = 0x7327,// AF3 
  
}ENCODER_PIN_enum;


extern volatile int32_t RAllPulse;
extern int16_t ECPULSE1;
extern int16_t ECPULSE2;

void EncInit (void);
void Encoder_Init(ENCODER_PIN_enum ap,ENCODER_PIN_enum bp);
int Read_Encoder(u8 TIMX);
int Read_DirEncoder(u8 TIMX);
void TestEncoderOLED(void);
void TestEncoderTFT(void);
void TestEncoderTimerTFT(void);
#endif

//



