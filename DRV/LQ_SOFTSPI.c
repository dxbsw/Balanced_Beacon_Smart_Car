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
CS   PE10
SCK  PE12
MISO PE13 
MOSI PE14

#define Soft_SPI_CS_PIN       GPIOE,GPIO_Pin_10     //CS 默认是PE11,如果使用编码器，则可以切换到PE10
#define Soft_SPI_SCK_PIN      GPIOE,GPIO_Pin_12     //SCK
#define Soft_SPI_MISO_PIN     GPIOE,GPIO_Pin_13     //单片机MISO接模块的--SDO
#define Soft_SPI_MOSI_PIN     GPIOE,GPIO_Pin_14     //单片机MOSI接模块的--SDI

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "string.h"
#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"
////////////////////////////////////////////////////////////////////////////////

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void Soft_SPI_Init(void)
【功  能】 模拟SPI初始化
【参数值】 无
【参数值】 无
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 Soft_SPI_Init()
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_Init(void)
{  
  /*
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  //默认使用SPI1 E11-14口
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);   //SPI2 clk enable  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11 | GPIO_Pin_12| GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  */
  
  //SPI_NSS  
  PIN_InitConfig(PE10, GPO, 1,GPIO_Mode_Out_PP);
  //SPI_SCK
  PIN_InitConfig(PE12, GPO, 1,GPIO_Mode_Out_PP);
  //SPI_MISO
  PIN_InitConfig(PE13, GPI, 1,GPIO_Mode_FLOATING);
  //SPI_MOSI
  PIN_InitConfig(PE14, GPO, 1,GPIO_Mode_Out_PP);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u8 len)
【功  能】 模拟SPI读写数据及长度
【参数值】 u8 *buf数据指针,u16 len长度
【参数值】 u8 *buf数据指针,u16 len长度
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Soft_SPI_ReadWriteNbyte(u8 *lqbuff, u8 len)
{
  u8 i;
  
  Soft_SPI_CS_OUT_L;
  Soft_SPI_SCK_OUT_H;
  do
  {
    for(i = 0; i < 8; i++)
    {
      if((*lqbuff) >= 0x80)
      {
        Soft_SPI_MOSI_OUT_H;        
      }
      else
      {
        Soft_SPI_MOSI_OUT_L;       
      }        
      
      Soft_SPI_SCK_OUT_L;
      (*lqbuff) <<= 1;      
      
      Soft_SPI_SCK_OUT_H;
      
      (*lqbuff) |= Soft_SPI_MISO_IN;          
    }
    lqbuff++;
  }while(--len);
  Soft_SPI_CS_OUT_H;
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
【功  能】SPI从设备读取数据
【参数值】SPI_TypeDef* SPIx, SPI通道选择
【参数值】unsigned char reg,设备起始地址
【参数值】unsigned char* buf,数据存放地址
【参数值】unsigned short len,待读取的数据长度
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Read(unsigned char reg, unsigned short len, unsigned char* buf)
{  
#ifdef USE_SOFT_SPI
  unsigned short i;
  buf[0] = reg | 0x80;  
  Soft_SPI_ReadWriteNbyte(buf, len + 1); 
  for(i=0;i<len;i++)
    buf[i]=buf[i+1];
#else
  SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
#endif
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
【功  能】SPI向设备写入数据
【参数值】
【参数值】unsigned char reg,设备起始地址
【参数值】unsigned char* buf,数据存放地址
【参数值】unsigned short len,待写入的数据长度
【返回值】无 
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQ_SPI_Write(unsigned char reg, unsigned short len, unsigned char* buf)
{   
#ifdef USE_SOFT_SPI
  unsigned short i;
  for(i=0;i<len;i++)
    buf[len-i]=buf[len-i-1];
  buf[0] = reg | 0x80;  
  
  Soft_SPI_ReadWriteNbyte(buf, len + 1);  
#else
  SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
#endif
}

