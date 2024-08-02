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
#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include "hal_exti.h"
#include "hal_misc.h"
#include "hal_syscfg.h"
#include "hal_gpio.h"

// GPIO 枚举端口编号
typedef enum //
{
  PA0 = 0, PA1,  PA2,  PA3,  PA4,  PA5,  PA6,  PA7,
  PA8,  PA9,  PA10, PA11, PA12, PA13, PA14, PA15,
  
  PB0,  PB1,  PB2,  PB3,  PB4,  PB5,  PB6,  PB7,
  PB8,  PB9,  PB10, PB11, PB12, PB13, PB14, PB15,
  
  PC0,  PC1,  PC2,  PC3,  PC4,  PC5,  PC6,  PC7,
  PC8,  PC9,  PC10, PC11, PC12, PC13, PC14, PC15,
  
  PD0,  PD1,  PD2,  PD3,  PD4,  PD5,  PD6,  PD7,
  PD8,  PD9,  PD10, PD11, PD12, PD13, PD14, PD15,
  
  PE0,  PE1,  PE2,  PE3,  PE4,  PE5,  PE6,  PE7,
  PE8,  PE9,  PE10, PE11, PE12, PE13, PE14, PE15,
  
  PF0,  PF1,  PF2,  PF3,  PF4,  PF5,  PF6,  PF7,
  PF8,  PF9,  PF10, PF11, PF12, PF13, PF14, PF15,
  
  PG0,  PG1,  PG2,  PG3,  PG4,  PG5,  PG6,  PG7,
  PG8,  PG9,  PG10, PG11, PG12, PG13, PG14, PG15,
  
  PH0,  PH1,  PH2,  PH3,  PH4,  PH5,  PH6,  PH7,
  PH8,  PH9,  PH10, PH11, PH12, PH13, PH14, PH15,  
  
}GPIO_Name_t;

typedef enum		// 枚举端口电平
{
  GPIO_AF0 = 0x00,//管脚复用0--15																
  GPIO_AF1 = 0x01,																		
  GPIO_AF2 = 0x02,																		
  GPIO_AF3 = 0x03,																			
  GPIO_AF4 = 0x04,																		
  GPIO_AF5 = 0x05,																			
  GPIO_AF6 = 0x06,																			
  GPIO_AF7 = 0x07,		
  GPIO_AF8 = 0x08,//管脚复用0--15																
  GPIO_AF9 = 0x09,																		
  GPIO_AF10 = 0x0A,																		
  GPIO_AF11 = 0x0B,																			
  GPIO_AF12 = 0x0C,																		
  GPIO_AF13 = 0x0D,																			
  GPIO_AF14 = 0x0E,																			
  GPIO_AF15 = 0x0F,	
}GPIO_AF_t;

typedef enum
{
  GPI = 0, //管脚输入
  GPO = 1, //管脚输出
}GPIO_DIR_t;

typedef enum		// 枚举端口方向
{
  GPI_AIN = 0x00,	// 引脚模拟输入
  GPO_PP  = 0x03,	// 引脚推挽输出
  GPI_FL  = 0x04,	// 引脚浮空输入
  GPO_OD  = 0x07,	// 引脚开漏输出
  GPI_PD  = 0x08,	// 引脚下拉输入
  GPI_PU  = 0x09,	// 引脚上拉输入
  GPO_AFPP = 0x0B,	// 引脚复用推挽输出
  GPO_AFOD = 0x0F,	// 引脚复用开漏输出
}GPIO_MODE_enum;


/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(pin) (GPIO_TypeDef *)PORTX[pin/16]

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(pin)   (unsigned char)(pin%16)
extern GPIO_TypeDef * PORTX[8];
void PIN_InitConfig(GPIO_Name_t pin, GPIO_DIR_t dir, unsigned char dat,unsigned short pinconf);
void PIN_AFIO_Init(GPIO_Name_t pin, GPIO_DIR_t dir,GPIO_MODE_enum mode,GPIO_AF_t af);
unsigned char PIN_Read(GPIO_Name_t pin);
void PIN_Write(GPIO_Name_t pin, BitAction dat);
void PIN_Dir(GPIO_Name_t pin, GPIO_DIR_t dir);
void PIN_Reverse(GPIO_Name_t pin);
void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,unsigned char prior, unsigned char sub_prior);

#endif /* 0_APPSW_TRICORE_APP_LQ_GPIO_H_ */
