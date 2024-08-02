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
#ifndef __LQ_SYSDELAY_H_
#define __LQ_SYSDELAY_H_
#include "include.h"

#ifdef __CC_ARM	
  #define ALIGN(n) __attribute__((aligned(n)))
#elif defined (__IAR_SYSTEMS_ICC__)  
  #define PRAGMA(x) _Pragma(#x)
  #define ALIGN(n) PRAGMA(data_alignment=n)
#elif defined (__GNUC__) 
  #define ALIGN(n) __attribute__((aligned(n)))
#endif 

#define true 1
#define false 0

extern u32 SystemCoreClock;
void delayus(u16 count);
void delayms(u16 nms);
void LQ_Init3227(void);
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status);
void DisableIRQ(IRQn_Type IRQn);
void EnableIRQ(IRQn_Type IRQn);

#endif



