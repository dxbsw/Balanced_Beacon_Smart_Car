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
#include <stdint.h>

/********************************************************************************************************
**函数信息 ：void delayms(u16 nms)    
**功能描述 ：延时nms
**输入参数 ：nms
**输出参数 ：
**    备注 ：SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK,72M条件下,nms<=1864 
********************************************************************************************************/
void delayus(u16 nus)
{
  u32 temp=SystemCoreClock/1000000;
  SysTick->CTRL = 0x0;                                                        //disable systick function
  SysTick->LOAD = nus *temp;                                                  //time nus for 1us with HSI as SYSCLK
  SysTick->VAL = 0x00;                                                        //clear counter
  SysTick->CTRL = 0x5;                                                        //start discrease with Polling
  do {
    temp = SysTick->CTRL;
  } while((temp & 0x01) && !(temp & (1 << 16)));                             //wait time nus done
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  //Close Counter
  SysTick->VAL = 0X00;                                                        //clear counter
}

void delayms(u16 nms)
{
  while(nms--)
  {
    delayus(1000);    
  }
}

/*************************************************************************
*  函数名称：void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
*  功能说明：中断初始化
*  参数说明：//  
//  @param      irqn              中断号，可以查看CH32V10x.h文件中的IRQn_Type枚举体定义
//  @param      pre_prior         抢占优先级,值越小优先级越高
//  @param      sub_prior         次优先级,值越小优先级越高
//  @param      status            使能或者禁止
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：    nvic_init(EXTI0_IRQn,1, 0, ENABLE); //外部中断1使能,抢占优先级次高，次优先级最高。
*************************************************************************/
void nvic_init(IRQn_Type irqn,u8 pre_prior, u8 sub_prior,u8 status)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_InitStructure.NVIC_IRQChannel = irqn;                        //中断号
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=pre_prior;   //抢占优先级,值越小优先级越高  		
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prior;        //子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd = (FunctionalState)status;  //使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
}

/**
\brief   Disable Interrupt
\details Disables a device specific interrupt in the NVIC interrupt controller.
\param [in]      IRQn  Device specific interrupt number.
\note    IRQn must not be negative.
*/
void DisableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0) {
    NVIC->ICER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
    __DSB();
    __ISB();
  }
}
/**
\brief   Enable Interrupt
\details Enables a device specific interrupt in the NVIC interrupt controller.
\param [in]      IRQn  Device specific interrupt number.
\note    IRQn must not be negative.
*/
void EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0) {
    NVIC->ISER[(((uint32_t)(int32_t)IRQn) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
  }
}
/********************************************************************************************************
**函数信息 ：void delay_init(void)                         
**功能描述 ：初始化延迟函数
**输入参数 ：
**输出参数 ：
**常用函数 ：
********************************************************************************************************/
void LQ_Init3227(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
  RCC_AHBPeriphClockCmd(0xFF, ENABLE);//开启PORTA--G口时钟  
  NVIC_SetPriority(SysTick_IRQn, 0x00);					                              
}