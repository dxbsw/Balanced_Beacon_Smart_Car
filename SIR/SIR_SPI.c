#include "include.h"
#include "string.h"
#include "HAL_conf.h"
#include "HAL_device.h"
#include "stdio.h"




/*
【函数名】 void SIR_SPI_Init(unsigned char SPI_N, SPI_BaudRatePrescaler_TypeDef spi_baud_div)
【功  能】 SPI初始化
【参数值】 SPI_TypeDef* SPIx, SPI通道选择
【参数值】 SPI_BaudRatePrescaler_TypeDef spi_baud_div, SPI波特率分频最大只能 SPI_BaudRatePrescaler_8
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
*/
void SIR_SPI_Init(unsigned char SPI_N)
{
  SPI_InitTypeDef SPI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;

  switch(SPI_N){
    case SPI_1:
    {
      //////////////////////////////////SPI时钟初始化/////////////////////////////////////  
      RCC_APB1PeriphClockCmd(RCC_APB2ENR_SPI1, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
      //////////////////////////////////SPI管脚初始化/////////////////////////////////////  
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
      
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_5); //SPI_SCK
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_5); //SPI_CS
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_5);//SPI_MISO
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_5);//SPI_MOSI
      
      /////////////////////////////////////SPI初始化////////////////////////////////////// 
      SPI_StructInit(&SPI_InitStructure);
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
      SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
      //	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
      SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)0x8;
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
      SPI_Init(SPI1, &SPI_InitStructure);
      if(SPI_InitStructure.SPI_BaudRatePrescaler <= 8) {
        exSPI_DataEdgeAdjust(SPI1, SPI_DataEdgeAdjust_FAST);
      }
      SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Rx);
      SPI_BiDirectionalLineConfig(SPI1, SPI_Direction_Tx);
      SPI_Cmd(SPI1, ENABLE);
    }break;
    
    case SPI_2:
    {
      //////////////////////////////////SPI时钟初始化/////////////////////////////////////  
      RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI2, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
      //////////////////////////////////SPI管脚初始化/////////////////////////////////////  
      GPIO_StructInit(&GPIO_InitStructure);
     // GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_6;
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_6;

      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOE, &GPIO_InitStructure);
      
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_5); //SPI_SCK
//      GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_5); //SPI_CS
//      GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_5);//SPI_MISO
      GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_5);//SPI_MOSI
      
      ///////////////////////////////////SPI初始化//////////////////////////////////////// 
      SPI_StructInit(&SPI_InitStructure);
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
      SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
      //	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
      SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)0x8;
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
      SPI_Init(SPI2, &SPI_InitStructure);
      if(SPI_InitStructure.SPI_BaudRatePrescaler <= 8) {
        exSPI_DataEdgeAdjust(SPI2, SPI_DataEdgeAdjust_FAST);
      }
      SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
      SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Tx);
      SPI_Cmd(SPI2, ENABLE);
    }break;
    
    case SPI_3:
    {
      //////////////////////////////////SPI时钟初始化/////////////////////////////////////  
      RCC_APB1PeriphClockCmd(RCC_APB1ENR_SPI3, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
      //////////////////////////////////SPI管脚初始化/////////////////////////////////////  
      GPIO_StructInit(&GPIO_InitStructure);
      GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
      GPIO_Init(GPIOD, &GPIO_InitStructure);
      
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_5); //SPI_SCK
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_5); //SPI_CS
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_5);//SPI_MISO
      GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_5);//SPI_MOSI
      
      /////////////////////////////////////SPI初始化////////////////////////////////////// 
      SPI_StructInit(&SPI_InitStructure);
      SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
      SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
      SPI_InitStructure.SPI_DataWidth = SPI_DataWidth_8b;
      SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
      SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
      SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
      //	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
      SPI_InitStructure.SPI_BaudRatePrescaler = (SPI_BaudRatePrescaler_TypeDef)0x8;
      SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
      SPI_Init(SPI3, &SPI_InitStructure);
      if(SPI_InitStructure.SPI_BaudRatePrescaler <= 8) {
        exSPI_DataEdgeAdjust(SPI3, SPI_DataEdgeAdjust_FAST);
      }
      SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Rx);
      SPI_BiDirectionalLineConfig(SPI3, SPI_Direction_Tx);
      SPI_Cmd(SPI3, ENABLE);
    }break;
  }
}

