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
#ifndef __LQ_Soft_SPI_H_
#define __LQ_Soft_SPI_H_
#include "include.h"

//                            单片机    龙邱20689模块
#define Soft_SPI_SCK_PIN      GPIOE,GPIO_Pin_12     //SCK
#define Soft_SPI_MISO_PIN     GPIOE,GPIO_Pin_13     //单片机MISO接模块的--SDO
#define Soft_SPI_MOSI_PIN     GPIOE,GPIO_Pin_14     //单片机MOSI接模块的--SDI
#define Soft_SPI_CS_PIN       GPIOE,GPIO_Pin_10     //CS 默认是PE11,如果使用编码器，则可以切换到PE10

#define Soft_SPI_SCK_OUT_H    GPIO_SetBits(Soft_SPI_SCK_PIN)          //配置输出高电平
#define Soft_SPI_SCK_OUT_L    GPIO_ResetBits(Soft_SPI_SCK_PIN)        //配置输出低电平

#define Soft_SPI_MISO_IN      GPIO_ReadInputDataBit(Soft_SPI_MISO_PIN)//读取引脚上的引脚状态

#define Soft_SPI_MOSI_OUT_H   GPIO_SetBits(Soft_SPI_MOSI_PIN)        //配置输出高电平
#define Soft_SPI_MOSI_OUT_L   GPIO_ResetBits(Soft_SPI_MOSI_PIN)      //配置输出低电平

#define Soft_SPI_CS_OUT_H     GPIO_SetBits(Soft_SPI_CS_PIN)          //配置输出高电平
#define Soft_SPI_CS_OUT_L     GPIO_ResetBits(Soft_SPI_CS_PIN)        //配置输出低电平

void Soft_SPI_Init(void);
void Soft_SPI_ReadWriteNbyte(unsigned char *lqbuff, unsigned char len);
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf);
void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf);
#endif

















