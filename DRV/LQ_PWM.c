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
#include "HAL_gpio.h"

extern GPIO_TypeDef * PORTX[8];

/*************************************************************************
*  函数名称：void PWM_PinConfig(PWM_Name_t pch)
*  功能说明：管脚初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：Set_TIM3PWM_Duty(1，1000);     // 电机驱动的PWM为12.5Khz，占空比为10%
*************************************************************************/
void PWM_PinConfig(PWM_Name_t pch)
{   
  PIN_AFIO_Init((GPIO_Name_t)(pch &0xFF), GPO, GPO_AFPP,(GPIO_AF_t)((pch &0x700)>>8));//PWM引脚初始化
}

/*************************************************************************
*  函数名称：void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
*  功能说明：电机PWM初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：PWM_InitConfig(PWM3_CH1_C6, 50, 5000);   //初始化PWM3 通道1 使用引脚C6 输出PWM频率50HZ 占空比为百分之 5000/PWM_DUTY_MAX*100
*************************************************************************/
void PWM_InitConfig(PWM_Name_t pch, u32 freq, u32 duty)
{		
  TIM_TimeBaseInitTypeDef  TIM_StructInit;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  u16 tmduty;                                               //占空比值
  u16 tmperiod;                                             //周期值
  u16 freq_div = 0;                                         //分频值
  
  PWM_PinConfig(pch);   
  
  if((pch >>13) == 0x00)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  //开启定时器1时钟
  else if((pch >>13) == 0x01)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  //开启定时器2时钟
  else if((pch >>13) == 0x02)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  //开启定时器3时钟
  else if((pch >>13) == 0x03)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM4, ENABLE);     //开启定时器
  else if((pch >>13) == 0x04)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM5, ENABLE);     //开启定时器
  else if((pch >>13) == 0x05)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6, ENABLE);     //开启定时器
  else if((pch >>13) == 0x06)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7, ENABLE);     //开启定时器
  else if((pch >>13) == 0x07)
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM8, ENABLE);     //开启定时器
  
  //获取系统主频
  //SystemCoreClock = 8000000 * (((RCC->CFGR >> 18)&0x0F) + 2);		
  
  freq_div = (u16)((SystemCoreClock / freq) >> 15);                //多少分频
  tmperiod = (u16)(SystemCoreClock/(freq*(freq_div + 1)));         //周期
  tmduty = tmperiod * duty / PWM_DUTY_MAX;                         //占空比
  
  if(freq_div<1) freq_div=1;
  if(tmperiod<1) tmperiod=1;
  
  //初始化TIM
  TIM_StructInit.TIM_Period = tmperiod - 1;                        //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_StructInit.TIM_Prescaler = freq_div-1;                         //设置用来作为TIMx时钟频率除数的预分频值
  TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
  TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
  TIM_StructInit.TIM_RepetitionCounter = 0;
  
  //初始化TIM1 PWM模式
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                       //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = tmduty;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;                //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;               //输出极性:TIM输出比较极性
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  
  TIM_TimeBaseInit(((TIM_TypeDef *) TIMERx[pch >>13]), &TIM_StructInit);  //初始化TIMx的时基
  
  if((pch&0x1800>>11) == 0x00)                                                //通道选择
  {
    TIM_OC1Init(((TIM_TypeDef *) TIMERx[pch >>13]), &TIM_OCInitStructure );                       //定时器通道1初始化
    TIM_OC1PreloadConfig(((TIM_TypeDef *) TIMERx[pch >>13]), TIM_OCPreload_Enable);               //定时器预装载配置        
  }
  else if((pch&0x1800>>11) == 0x01)
  {
    TIM_OC2Init(((TIM_TypeDef *) TIMERx[pch >>13]), &TIM_OCInitStructure );
    TIM_OC2PreloadConfig(((TIM_TypeDef *) TIMERx[pch >>13]), TIM_OCPreload_Enable);
    
  }
  else if((pch&0x1800>>11) == 0x02)
  {
    TIM_OC3Init(((TIM_TypeDef *) TIMERx[pch >>13]), &TIM_OCInitStructure );
    TIM_OC3PreloadConfig(((TIM_TypeDef *) TIMERx[pch >>13]), TIM_OCPreload_Enable);        
  }
  else if((pch&0x1800>>11) == 0x03)
  {
    TIM_OC4Init(((TIM_TypeDef *) TIMERx[pch >>13]), &TIM_OCInitStructure );
    TIM_OC4PreloadConfig(((TIM_TypeDef *) TIMERx[pch >>13]), TIM_OCPreload_Enable);        
  }
  
  TIM_ARRPreloadConfig(((TIM_TypeDef *) TIMERx[pch >>13]), ENABLE );                                               
  TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERx[pch >>13]), ENABLE );
  TIM_Cmd(((TIM_TypeDef *) TIMERx[pch >>13]), ENABLE);   //定时器使能
}

/*************************************************************************
*  函数名称：void Set_TIM3PWM_Duty(u8 ch,u16 duty)
*  功能说明：电机PWM初始化
*  参数说明：pch,PWM通道所对应的的定时器及管脚
u32 duty占空比为百分之duty*100/PWM_DUTY_MAX
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：PWM_Set_Duty(PWM3_CH1_PC6, 5000);
*************************************************************************/
void PWM_Set_Duty(PWM_Name_t pch, u32 duty)
{
  switch(pch&0x1800)
  {
  case 0x0000: ((TIM_TypeDef *) TIMERx[pch >>13])->CCR1=duty;break;
  case 0x0800: ((TIM_TypeDef *) TIMERx[pch >>13])->CCR2=duty;break;
  case 0x1000: ((TIM_TypeDef *) TIMERx[pch >>13])->CCR3=duty;break;
  case 0x1800: ((TIM_TypeDef *) TIMERx[pch >>13])->CCR4=duty;break;
  default: break;;
  }  
}

/*************************************************************************
*  函数名称：void TIM8PWM_Init(u16 freq, u16 dutyCycle)
*  功能说明：舵机PWM初始化
*  参数说明：u16 freq, 要合理，内核频率约为94M，理论应该是96M,不准确
u16 dutyCycle 0--TIM3_PWM_DUTY_MAX 10000对应0--100%
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：TIM8PWM_Init(100，1500);     // 舵机驱动的PWM为100hz，占空比为1.5ms
*************************************************************************/
void TIM8PWM_Init(u16 freq, u16 dutyCycle)
{
  TIM_TimeBaseInitTypeDef TIM_StructInit;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM8, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_3); 
  
  //TIM8_CH1
  GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);  
  
  TIM_TimeBaseStructInit(&TIM_StructInit);
  TIM_StructInit.TIM_Period = SystemCoreClock/freq;
  TIM_StructInit.TIM_Prescaler = 0; //不分频，分频值为；n+1
  TIM_StructInit.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_StructInit.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_StructInit.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM8, &TIM_StructInit);
  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = dutyCycle*TIM_StructInit.TIM_Period/PWM_DUTY_MAX;//占空比设置
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);                    //只用了一个通道
  //TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  //TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  //TIM_OC4Init(TIM8, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);           //只用了一个通道
  //TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
  //TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
  //TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
  
  TIM_ARRPreloadConfig(TIM8, ENABLE);    
  TIM_CtrlPWMOutputs(TIM8, ENABLE);
  TIM_Cmd(TIM8, ENABLE);   
}
