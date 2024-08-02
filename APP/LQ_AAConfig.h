#ifndef __LQ_AAConfig_H__
#define __LQ_AAConfig_H__
#include "include.h"

//#define USEOLED        //使用OLED或者TFT1.8模块，默认为TFT1.8
#define USETFT           //默认为TFT1.8

#define USE_SOFT_SPI //使用模拟SPI,硬件SPI不通

//#define LQ_ICM20689_I2CDMP 
//#define LQ_ICM20689_SPIDMP
//#define USEMPU6050DMP  


#define USETIMER1DMA     //开启此项使用DMA传输，关闭此项则使用传感的中断方式采集

#define LQMT9V034        //默认为神眼摄像头
#define USELQMT9V034     //使用神眼摄像头

#define IMAGEH  MT9V034_IMAGEH   //摄像头采集高度
#define IMAGEW  MT9V034_IMAGEW   //摄像头采集宽度

// UART Printf Definition
#define DEBUG_UART1    1
#define DEBUG_UART2    2
#define DEBUG_UART3    3

// DEBUG UATR Definition
#define DEBUG   DEBUG_UART1
// #define DEBUG   DEBUG_UART2
// #define DEBUG   DEBUG_UART3


//GPIO寄存器操作宏定义
#define GPIO_IDR_ADDR(p)	(0x40040008 + ((p&0xf0)>>4)*0x400 + (p&0x0f))
#define GPIO_ODR_ADDR(p)	(0x4004000C + ((p&0xf0)>>4)*0x400 + (p&0x0f))
#define GPIO_BSRR_ADDR(p)	(0x40040010 + ((p&0xf0)>>4)*0x400 + (p&0x0f))
#define GPIO_BRR_ADDR(p)	(0x40040014 + ((p&0xf0)>>4)*0x400 + (p&0x0f))


 
#endif









