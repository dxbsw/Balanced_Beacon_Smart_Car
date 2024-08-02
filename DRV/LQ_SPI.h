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
#ifndef __LQ_SPI_H_
#define __LQ_SPI_H_
#include "include.h"


typedef enum //SPI序号
{
  SPI_1,
  SPI_2,
  SPI_3,
}SPIn_e;

typedef enum //SCK复用管脚
{
  SPI1_SCK_A05= 0x5005,// AF5 保留 PA口 A5
  SPI1_SCK_B03= 0x5013,
  SPI1_SCK_E12= 0x504C,
  
  SPI2_SCK_B10= 0x501A,				
  SPI2_SCK_B13= 0x501D,				
  SPI2_SCK_D03= 0x5033,				
  SPI2_SCK_E02= 0x5042,				
  
  SPI3_SCK_B03= 0x6013,				
  SPI3_SCK_C10= 0x602A,				
  SPI3_SCK_D04= 0x5034,				
  SPI3_SCK_D12= 0x603C,			
}SPI_SCK_e;


typedef enum//MOSI复用管脚
{
  SPI1_MOSI_A07= 0x5007,		
  SPI1_MOSI_B05= 0x5015,	
  SPI1_MOSI_E14= 0x504E,	
  
  SPI2_MOSI_B15= 0x501F,		
  SPI2_MOSI_C03= 0x5023,
  SPI2_MOSI_E06= 0x5046,				
  
  SPI3_MOSI_B05= 0x6015,		
  SPI3_MOSI_C12= 0x602C,	
  SPI3_MOSI_D06= 0x5036,				
  SPI3_MOSI_D14= 0x603E,		
}SPI_MOSI_e;


typedef enum//MISO复用管脚
{
  SPI1_MISO_A06= 0x5006,	
  SPI1_MISO_B04= 0x5014,
  SPI1_MISO_E13= 0x504D,
  
  SPI2_MISO_B14= 0x501E,
  SPI2_MISO_C02= 0x5022,	
  SPI2_MISO_C06= 0x5026,	
  SPI2_MISO_E05= 0x5045,	
  
  SPI3_MISO_B04= 0x6014,	
  SPI3_MISO_C07= 0x5027,	
  SPI3_MISO_C11= 0x602B,	
  SPI3_MISO_D05= 0x5035,
  SPI3_MISO_D13= 0x603D,
}SPI_MISO_e;


typedef enum//CS复用管脚
{	
  SPI1_NSS_A04= 0x5004,			
  SPI1_NSS_A15= 0x500F,
  SPI1_NSS_E11= 0x504B,		
  
  SPI2_NSS_B09= 0x5019,		
  SPI2_NSS_B12= 0x501C,		
  SPI2_NSS_E03= 0x5043,	
  SPI2_NSS_E04= 0x5044,	
  
  SPI3_NSS_A04= 0x6004,				
  SPI3_NSS_A15= 0x600F,				
  SPI3_NSS_D07= 0x5037,			
  SPI3_NSS_D15= 0x603F,		
}SPI_CS_e;

unsigned char SPI_WriteRead(SPI_TypeDef* SPIx, unsigned char tx_data);
void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div);
void SPI_Read_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len);
void SPI_Write_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len);
void SPIM_TXEn(SPI_TypeDef* SPIx);
void SPIM_RXEn(SPI_TypeDef* SPIx);
void SPIM_CSLow(SPI_TypeDef* SPIx);
void SPIM_CSHigh(SPI_TypeDef* SPIx);


#endif

















