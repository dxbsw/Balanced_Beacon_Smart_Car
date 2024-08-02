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
#ifndef __LQ_ADC_H_
#define __LQ_ADC_H_

#include "include.h"
#include "hal_adc.h"
#include "hal_rcc.h"

/* ADC Software start mask */
#define CTLR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
#define CTLR2_EXTTRIG_SWSTART_Reset   ((uint32_t)0xFFAFFFFF)

void ADC1Init(ADCCHANNEL_TypeDef ADC_Channel_x);
u16 ADC1_Read(uint8_t ADC_Channel_x);
u16 ADC1_Read_Average(uint8_t ADC_Channel_x,uint8_t times);

void ADC2Init(ADCCHANNEL_TypeDef ADC_Channel_x);
u16 ADC2_Read(uint8_t ADC_Channel_x);
u16 ADC2_Read_Average(uint8_t ADC_Channel_x,uint8_t times);

void ADC3Init(ADCCHANNEL_TypeDef ADC_Channel_x);
u16 ADC3_Read(uint8_t ADC_Channel_x);
u16 ADC3_Read_Average(uint8_t ADC_Channel_x,uint8_t times);

void Test_ADC(void);

#endif
//



