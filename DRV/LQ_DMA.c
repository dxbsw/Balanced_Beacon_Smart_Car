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
#include "include.h"
#include "hal_rcc.h"
#include "hal_tim.h"

void DMA1_Channel1_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC1)!=RESET)										
  {
    DMA_ClearFlag(DMA1_FLAG_TC1);        
    LED_Ctrl(LED2,RVS);
  }
}

void DMA1_Channel2_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC2)!=RESET)										
  {
    DMA_ClearFlag(DMA1_FLAG_TC2);												
  }
}

void DMA1_Channel3_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC3)!=RESET)										
  {
    DMA_ClearFlag(DMA1_FLAG_TC3);	
    
  }
}
void DMA1_Channel4_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC4)!=RESET)
  {
    DMA_ClearFlag(DMA1_FLAG_TC4);   
    MT9V034_DMA_CLS();  //关闭DMA1,采集标志置一
  }
}
void DMA1_Channel5_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC5)!=RESET)
  {
    DMA_ClearFlag(DMA1_FLAG_TC5);   
    
  }
}
/*************************************************************************
* @brief    摄像头DMA传输初始化
*
* @param    srcStartAddr   ：源地址 IO地址
* @param    dstStartAddr   ：目的地址 摄像头数据缓冲区
* @param    channel        ：DMA传输通道 0-12    注意 DMA传输通道需要和PIX像素中断优先级保持一致
* @param    lqsize          数据长度
* @return   无
*
* @note     注意 DMA传输通道需要和PIX像素中断优先级保持一致
*
* @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
*
* @date     2020/4/8
*************************************************************************/
void DMA_CameraInitConfig(DMA_Channel_TypeDef* dma_ch,u32 srcStartAddr, u32 dstStartAddr, u32 lqsize)
{
  DMA_InitTypeDef DMA_InitStructure;  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1总线初始化  
  
  DMA_DeInit(dma_ch);

  //MDA配置初始化
  DMA_InitStructure.DMA_PeripheralBaseAddr = srcStartAddr;                //源地址，DMA外设地址
  DMA_InitStructure.DMA_MemoryBaseAddr = dstStartAddr;                    //目标地址，DMA内存地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      //方向(从外设到内存)，外设作为数据传输的来源
  DMA_InitStructure.DMA_BufferSize = lqsize;                              //指定DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设地址固定
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //内存地址递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设每次传输一个字节
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //内存每次传输一个字节
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           //非循环模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //优先级最高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            //禁止内存到内存的传输
  DMA_InitStructure.DMA_Auto_reload = DMA_Auto_Reload_Enable;             //自动重载  
  DMA_Init(dma_ch, &DMA_InitStructure);                                   //初始化
  
  DMA_ITConfig(dma_ch, DMA_IT_TC, ENABLE);                                //使能传输完成中断
  nvic_init(MT9V034_DMA_IRQN, 0, 1, ENABLE);                              //中断配置
  DMA_Cmd(dma_ch, ENABLE);                                                //开启DMA1
}

/*************************************************************************
//  @brief      摄像头GPIO触发定时器初始化
//  @param      timern 定时器
//  @param      pin    引脚号
* @return   无
* @see     DMA_CameraTriggerTimerInit(TIMER_1, MT9V034_PCLK_PIN); //GPIO触发TIM引脚固定
*
* @date     2020/4/8
* 修改触发管脚，需要修改相应的中断配置等。
*************************************************************************/
void DMA_CameraTriggerTimerInit(TIMER_Name_t timern, GPIO_Name_t pin)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
  
  if(TIMER_1 == timern)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  else if(TIMER_2 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  else if(TIMER_3 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  else if(TIMER_4 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  else if(TIMER_5 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM5, ENABLE);
  else if(TIMER_6 == timern)//无效
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6, ENABLE);
  else if(TIMER_7 == timern)//无效
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7, ENABLE);
  else if(TIMER_8 == timern)
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM8, ENABLE);
  
  //像素时钟管脚复用  
  PIN_InitConfig(pin, GPI, 0, GPIO_Mode_IN_FLOATING);  
  GPIO_PinAFConfig((GPIO_TypeDef *)(PORTX[(pin&0x00F0) >> 4]),  pin & 0x0F, GPIO_AF1);    
  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit((TIM_TypeDef*)(TIMERx[timern]), &TIM_TimeBaseStructure);
  
  //TIM_ExtTRGPolarity_Inverted  //TIM_ExtTRGPolarity_NonInverted
  TIM_ETRClockMode1Config((TIM_TypeDef*)(TIMERx[timern]), TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  TIM_SelectInputTrigger((TIM_TypeDef*)(TIMERx[timern]), TIM_TS_ETRF);
  TIM_SelectSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_SlaveMode_Reset);  
  TIM_SelectMasterSlaveMode((TIM_TypeDef*)(TIMERx[timern]), TIM_MasterSlaveMode_Enable);
  
  TIM_Cmd((TIM_TypeDef*)(TIMERx[timern]), ENABLE);
  TIM_DMACmd((TIM_TypeDef*)(TIMERx[timern]), TIM_DMA_Trigger, ENABLE);
}

//简单使用PE7测试
void DMA_CameraTriggerTimer1PE7Init()
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
  
  PIN_InitConfig(PE7, GPI, 0, GPIO_Mode_IN_FLOATING);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF1);
  
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
  
  //TIM_ExtTRGPolarity_Inverted  //TIM_ExtTRGPolarity_NonInverted
  TIM_ETRClockMode1Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x00);
  TIM_SelectInputTrigger(TIM1, TIM_TS_ETRF);      //External Trigger input
  TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset); // Rising edge of the selected trigger signal (TRGI) re-initializes
                                                  // the counter and triggers an update of the registers.
  TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);//synchronization between the current timer
                                                              // and its slaves (through TRIG).
  
  TIM_Cmd(TIM1, ENABLE);
  TIM_DMACmd(TIM1, TIM_DMA_Trigger, ENABLE);
}

