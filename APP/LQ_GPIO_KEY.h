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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

#include "include.h"

//定义模块号
typedef enum
{
  KEY0 = 0,  //母板上按键0
  KEY1 = 1,
  KEY2 = 2,
  DSW0 = 3,    //母板上拨码开关0
  DSW1 = 4,
} KEYn_e;

typedef enum
{
  LOW = 0,  //按下
  HIGH = 1, //松开
  FAIL = 0xff, //错误
} KEYs_e;

//定义的管脚要对应实际按键
#define KEY0p      PD4//GPIOD,GPIO_Pin_4   //母板上按键0
#define KEY1p      PD2//GPIOD,GPIO_Pin_2   //母板上按键1
#define KEY2p      PD3//GPIOD,GPIO_Pin_3   //母板上按键2
#define DSW0p      PD0//GPIOD,GPIO_Pin_0   //母板上拨码开关0,默认拨到下方是高电平，上方为低电平
#define DSW1p      PD1//GPIOD,GPIO_Pin_1  //母板上拨码开关1
#define REEDp      GPIOA,GPIO_Pin_2   //母板上干簧管
/*********************** UART功能函数 **************************/
//初始化
void GPIO_KEY_Init (void);
int KEY_Read_All (void);
/*************************************************************************
*  函数名称：void Reed_Init(void)
*  功能说明：干簧管GPIO及中断初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月21日
*  备    注：   中断在那个核初始化，中断VECTABNUM号必须对应该cpu,否则不会调用中断函数
*          中断服务函数PIN_INT0_IRQHandler在LQ_GPIO.c中
*************************************************************************/
void Reed_Init(void);
u8 KEY_Scan(u8 mode);
unsigned char KEY_Read(KEYn_e KEYno);


#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
