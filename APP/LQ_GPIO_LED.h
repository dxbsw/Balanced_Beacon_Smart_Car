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
#ifndef __LQ_LED_H_
#define __LQ_LED_H_

#include "include.h"

//定义模块号
typedef enum
{
  LED0=0,  //核心板上LED0
  LED1=1,  //核心板上LED1
  LED2=2,  //母板上LED0
  LED3=3,   //母板上LED1
  LEDALL=4
} LEDn_e;

typedef enum
{
  ON=0,  //亮
  OFF=1, //灭
  RVS=2, //反转
}LEDs_e;

//定义的管脚要对应实际按键
#define LED0p      GPIOF,GPIO_Pin_14  //核心板上LED0 翠绿
#define LED1p      GPIOF,GPIO_Pin_10  //核心板上LED1 蓝灯
#define LED2p      GPIOE,GPIO_Pin_0   //母板上预留LED翠绿
#define LED3p      GPIOE,GPIO_Pin_1   //母板上预留LED蓝灯

/*********************** UART功能函数 **************************/
//初始化
void GPIO_LED_Init(void);
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta);
void Test_GPIO_LED(void);
#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
