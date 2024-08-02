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

硬件使用SPI1
SCK  PE12
MOSI PE13
MISO PE14 
CS   PE10
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "string.h"
#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"
////////////////////////////////////////////////////////////////////////////////


#define SPI_CS_PIN       GPIOE,GPIO_Pin_10           //CS 默认是PE11,如果使用编码器，则可以切换到PE10
#define SPI_CS_OUT_H     GPIO_SetBits(SPI_CS_PIN)    //配置输出高电平
#define SPI_CS_OUT_L     GPIO_ResetBits(SPI_CS_PIN)  //配置输出低电平

#define Dummy_Byte       0x00

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void SPI_Read_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len)
【功  能】SPI从设备读取数据
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【参数值】unsigned char Addr,设备地址
【参数值】unsigned char* buf,数据存放地址
【参数值】unsigned short len,待读取的数据长度
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPI_Read_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len)
{
  SPIM_CSLow(SPIx);
  //Send destination address 0-7 bits
  SPI_WriteRead(SPIx, Addr);
  len++;
  while (len--)
  {
    buf++;
    //Save the data read from flash into the specified array
    *buf = SPI_WriteRead(SPIx, Dummy_Byte);        
  }
  SPIM_CSHigh(SPIx);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void SPI_Write_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len)
【功  能】SPI向设备写入数据
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【参数值】unsigned char Addr,设备地址
【参数值】unsigned char* buf,数据存放地址
【参数值】unsigned short len,待写入的数据长度
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPI_Write_Nbytes(SPI_TypeDef* SPIx, unsigned char Addr, unsigned char* buf, unsigned short len)
{    
  SPIM_CSLow(SPIx);
  SPI_WriteRead(SPIx, Addr);
  len++;
  while (len--)
  {
    buf++;
    //Save the data read from flash into the specified array
    *buf = SPI_WriteRead(SPIx, Dummy_Byte);        
  }
  SPIM_CSHigh(SPIx);    
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void SPIM_CSLow(SPI_TypeDef* SPIx)
【功  能】SPI片选
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPIM_CSLow(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPIM_CSLow(SPI_TypeDef* SPIx)
{
  SPI_CSInternalSelected(SPIx,  ENABLE);     
  SPI_CS_OUT_L;//配置输出低电平
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void SPIM_TXEn(SPI_TypeDef* SPIx)
【功  能】SPI发送使能
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPIM_TXEn(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPIM_TXEn(SPI_TypeDef* SPIx)
{
  SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Tx);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void SPIM_RXEn(SPI_TypeDef* SPIx)
【功  能】 SPI接收使能
【参数值】 SPI_TypeDef* SPIx, SPI通道选择
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_RXEn(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPIM_RXEn(SPI_TypeDef* SPIx)
{
  SPI_BiDirectionalLineConfig(SPIx, SPI_Direction_Rx);
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void SPIM_CSHigh(SPI_TypeDef* SPIx)
【功  能】 SPI释放设备
【参数值】 SPI_TypeDef* SPIx, SPI通道选择
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPIM_CSHigh(SPI_TypeDef* SPIx)
{
  SPI_CSInternalSelected(SPIx, DISABLE);  
  SPI_CS_OUT_H;//配置输出高电平  
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】unsigned char SPI_WriteRead(SPI_TypeDef* SPIx, unsigned char tx_data)
【功  能】SPI读写数据
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【参数值】unsigned char tx_data,待写入的数据
【返回值】从设备读取的数据
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 x=SPI_Read_Nbytes(SPI1,buf);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char SPI_WriteRead(SPI_TypeDef* SPIx, unsigned char tx_data)
{
  SPI_SendData(SPIx, tx_data);
  while(!(SPIx->CSTAT & SPI_FLAG_TXEPT));
  while(!(SPIx->CSTAT & SPI_CSTAT_RXAVL));
  return (unsigned char)SPIx->RXREG;
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div)
【功  能】 SPI初始化
【参数值】 SPI_TypeDef* SPIx, SPI通道选择
【参数值】 SPI_BaudRatePrescaler_TypeDef spi_baud_div, SPI波特率分频
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void SPIM_Init(SPI_TypeDef* SPIx, SPI_BaudRatePrescaler_TypeDef spi_baud_div)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;

//////////////////////////////////SPI管脚初始化/////////////////////////////////////  
  //默认使用SPI1 E11-14口
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);   //SPI2 clk enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  //SPI_NSS
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_5);
  //SPI_SCK
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_5);
  //SPI_MISO
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_5);
  //SPI_MOSI
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_5);
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
///////////////////////////////////////////////////////////////////////////    
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  //	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = spi_baud_div;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_Init(SPIx, &SPI_InitStructure);
  
  SPI_Cmd(SPIx, ENABLE);
  SPIM_TXEn(SPIx);
  SPIM_RXEn(SPIx);
  
  //SPI_NSS  
  PIN_InitConfig(PE10, GPO, 0,GPIO_Mode_Out_PP);
}



