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

extern unsigned char Image_Data[IMAGEH][IMAGEW];;
GPIO_TypeDef * PORTX[8] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH};

u16 hangcnt=0;
u16 hang=0,lie=0;


void EXTI0_IRQHandler(void)
{
  //清除EXTI线路挂起位//
  EXTI_ClearFlag(EXTI_Line0);   
  //LED_Ctrl(LED1,RVS);
}

void EXTI1_IRQHandler(void)
{   
  //清除EXTI线路挂起位//
  EXTI_ClearFlag(EXTI_Line1); 

  MT9V034_DMA_START();   //PG1 摄像头场同步中断    

  LED_Ctrl(LED1,RVS);    //翠绿灯闪烁 
}

// 干簧管A2
void EXTI2_IRQHandler(void)
{
  //清除EXTI线路挂起位//
  EXTI_ClearFlag(EXTI_Line2); 

  //起跑，停车检测
  LED_Ctrl(LED1,RVS);
}

void EXTI4_IRQHandler(void)
{
  //清除EXTI线路挂起位//
  EXTI_ClearFlag(EXTI_Line4);   
  //LED_Ctrl(LED1,RVS);
}

void EXTI9_5_IRQHandler(void)
{ //清除EXTI5线路挂起位//

#ifdef USETIMER1DMA   
  if(EXTI_GetITStatus(EXTI_Line5))	//读取对应管的中断状态
  {
    EXTI_ClearFlag(EXTI_Line5);		  //清除中断标志
  }
  if(EXTI_GetITStatus(EXTI_Line6))
  {
    EXTI_ClearFlag(EXTI_Line6);			
  }
  
  if(EXTI_GetITStatus(EXTI_Line7))    //像素信号		
  {    	    
    EXTI_ClearFlag(EXTI_Line7); 
    
  } 
  if(EXTI_GetITStatus(EXTI_Line8))  	
  {
    EXTI_ClearFlag(EXTI_Line8);     
  }
#else  
  //行中断  
  EXTI_ClearFlag(EXTI_Line8); 
  if(hang%2==0)
  {
    for(lie=0;lie<LCDW;lie++)
    {
      Image_Data[hangcnt][lie]=(u8)(GPIOF->IDR);
    }
    if(++hangcnt>LCDH)
    {
      hangcnt=LCDH;
      Camera_Flag=1;
    }
  }
  ++hang;
  
#endif  
  
//  {
//    EXTI_ClearFlag(EXTI_Line9);			
//  }  	
//  
//  LED_Ctrl(LED1,RVS);
}

void EXTI10_15_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10))	//读取对应管的中断状态			
  {
    EXTI_ClearFlag(EXTI_Line10);    //清除中断标志
  }
  if(EXTI_GetITStatus(EXTI_Line11))
  {
    EXTI_ClearFlag(EXTI_Line11);	
  }
  if(EXTI_GetITStatus(EXTI_Line12))		
  {
    EXTI_ClearFlag(EXTI_Line12);		
  }
  if(EXTI_GetITStatus(EXTI_Line13))	
  {
    EXTI_ClearFlag(EXTI_Line13);		
  }
  if(EXTI_GetITStatus(EXTI_Line14))	
  {
    EXTI_ClearFlag(EXTI_Line14);		
  }
  if(EXTI_GetITStatus(EXTI_Line15))	
  {
    EXTI_ClearFlag(EXTI_Line15);		
  }
  //LED_Ctrl(LED1,RVS);
}

/*************************************************************************
*  函数名称：void PIN_InitConfig(GPIO_Name_t pin, GPIO_DIR_t dir, unsigned char dat,unsigned short pinconf)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*            u32 pinconf :管脚配置,参考ch32v10x_gpio.h中的枚举GPIO_Mode_AF_PP等
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);  //PA3初始化推挽输出 高电平
*************************************************************************/
void PIN_InitConfig(GPIO_Name_t pin, GPIO_DIR_t dir, unsigned char dat,unsigned short pinconf)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  u8 port_index = pin >> 4;
  
  //GPIO A--H总线时钟使能  
  RCC_AHBPeriphClockCmd(0xFF, ENABLE);
  
  //设置管脚索引
  GPIO_InitStructure.GPIO_Pin = (u16)(1 << (pin & 0x0F));
  
  if(GPO == dir)
  {  //设置GPIO速度
    if((u16)pinconf >> 8 == 0) GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)(GPIO_Speed_50MHz);  //防止没有填写这个参数,默认设置50Mhz
    else GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)((u16)pinconf >> 8);                       //已经填写了该参数。根据值来设置
  }
  
  //设置模式 ：输入、输出、上拉、复用、悬空、AD等
  GPIO_InitStructure.GPIO_Mode = (GPIOMode_TypeDef)((u16)pinconf & 0xff);
  GPIO_Init((GPIO_TypeDef *)(PORTX[port_index]), &GPIO_InitStructure);
  
  if(GPO == dir)//设置输出高低电平，高电平默认，作为输入口不设置
  {
    GPIO_WriteBit((GPIO_TypeDef *)(PORTX[port_index]), (u16)(1 << (pin & 0x0F)), (BitAction)dat);
  }
}
/*************************************************************************
*  函数名称：void PIN_AFIO_Init(GPIO_Name_t pin, GPIO_MODE_enum mode,GPIO_AF_t af)
*  功能说明：GPIO功能复用初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式  
*            GPIO_MODE_enum mode： GPIO 模式   lq_gpio.h中的宏定义中选择
*            GPIO_AF_t af管脚配置,参考_gpio.h中的枚举GPIO_Mode_AF_PP等
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_InitConfig(PA3, GPO, 1,GPIO_Mode_Out_PP);  //PA3初始化推挽输出 高电平
*************************************************************************/
void PIN_AFIO_Init(GPIO_Name_t pin, GPIO_DIR_t dir,GPIO_MODE_enum mode,GPIO_AF_t af)
{
  unsigned char port_index =(pin&0xF0)>>4;	
  unsigned char pin_index = pin&0x0F;	  
  
  RCC->AHBENR |= RCC_AHBENR_GPIOA << port_index;
  
  if(pin_index < 8)// 低8位管脚
  {
    PORTX[port_index]->CRL &= ~(0x0000000f << (pin_index*4));		
    PORTX[port_index]->CRL |= (((u32)dir|(u32)mode) << (pin_index*4));					
    PORTX[port_index]->AFRL &= ~(0x0000000f << (pin_index*4));	
    PORTX[port_index]->AFRL |= ((u32)af << (pin_index*4));		 
  }
  else	// 高8位管脚
  {
    PORTX[port_index]->CRH &= ~(0x0000000f << ((pin_index-8)*4));			
    PORTX[port_index]->CRH |= (((u32)dir|(u32)mode) << ((pin_index-8)*4));				
    PORTX[port_index]->AFRH &= ~(0x0000000f << ((pin_index-8)*4));			
    PORTX[port_index]->AFRH |= ((u32)af << ((pin_index-8)*4));				
  }
}
/*************************************************************************
*  函数名称：unsigned char PIN_Read(GPIO_Name_t pin)
*  功能说明：读取GPIO电平函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：uint8_t pinState = PIN_Read(PA2);  //读取PA2 的电平状态
*************************************************************************/
unsigned char PIN_Read(GPIO_Name_t pin)
{
  if((((GPIO_TypeDef *)(PORTX[pin>>4]))->IDR & (1<<((pin & 0x0F)))) != 0)
  {
    return 1;
  }
  return 0;
}

/*************************************************************************
*  函数名称：void PIN_Write(GPIO_Name_t pin, BitAction dat)
*  功能说明：设置GPIO输出状态
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*            dat   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Write(PE0, 1);//PE0置为高电平
*************************************************************************/
void PIN_Write(GPIO_Name_t pin, BitAction dat)
{
  if (dat == 1)
    ((GPIO_TypeDef *)(PORTX[pin>>4]))->BSRR = (1<<((pin & 0x0F)));
  else
    ((GPIO_TypeDef *)(PORTX[pin>>4]))->BRR = (1<<((pin & 0x0F)));
}

/*************************************************************************
*  函数名称：void PIN_Reverse(GPIO_Name_t pin)
*  功能说明：GPIO电平翻转函数，使用前请先初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
*************************************************************************/
void PIN_Reverse(GPIO_Name_t pin)
{
  ((GPIO_TypeDef *)(PORTX[pin>>4]))->ODR ^= (u16)(1<<((pin & 0x0F)));
}

/*************************************************************************
*  函数名称：void PIN_Dir(PIN_enum pin, GPIO_DIR_t dir)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式   lq_gpio.h中的宏定义中选择*            
*            
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_Dir(PA3, GPO);  //PA3初始化推挽输出 高电平
*************************************************************************/
void PIN_Dir(GPIO_Name_t pin, GPIO_DIR_t dir)
{
  unsigned char port_index = ((pin&0xf0)>>4);		                        // 提取GPIO分组
  unsigned char pin_index = (pin&0x0F);					// 提取GPIO引脚
  if(pin_index < 0x08)								          // 低8位
  {
    PORTX[port_index]->CRL &= ~(0x0000000f << (pin_index*4));			// 清空
    PORTX[port_index]->CRL |= ((u32)dir << (pin_index*4));			// 置位
  }
  else// 高8位
  {
    PORTX[port_index]->CRH &= ~(0x0000000f << ((pin_index-8)*4));		// 清空
    PORTX[port_index]->CRH |= ((u32)dir << ((pin_index-8)*4));			// 置位
  }
}

/*************************************************************************
*  函数名称：void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,unsigned char prior, unsigned char sub_prior)
*  功能说明：GPIO外部中断初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            dir     ： GPIO 模式   lq_gpio.h中的宏定义中选择*
*            u32 pinconf :管脚配置,参考ch32v10x_gpio.h中的枚举GPIO_Mode_AF_PP等
*            unsigned char prior 中断优先级
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Exti(PA2, EXTI_Trigger_Falling,GPIO_Mode_IPU);//配置PA2下降沿触发中断
*  备    注：
*************************************************************************/
void PIN_Exti(GPIO_Name_t pin, EXTITrigger_TypeDef edge,u32 pinconf,unsigned char prior, unsigned char sub_prior)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  
  GPIO_PinAFConfig((GPIO_TypeDef *)(PORTX[pin>>4]), (pin & 0x0F), GPIO_AF_0);  
  
  PIN_InitConfig(pin, GPI, 0, pinconf);
  SYSCFG_EXTILineConfig(pin>>4, (pin & 0x0F));            //管脚中断号配置
  EXTI_InitStructure.EXTI_Line = 1<<((pin & 0x0F));       //管脚号配置
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;     //触发模式
  EXTI_InitStructure.EXTI_Trigger = edge;                 //触发方式
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //使能或者失能
  EXTI_Init(&EXTI_InitStructure);
  
  // 中断分配
  if((pin&0x0F) < 1)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;								
  else if((pin&0x0F) < 2)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;									
  else if((pin&0x0F) < 3)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  else if((pin&0x0F) < 4)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  else if((pin&0x0F) < 5)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;									
  else if((pin&0x0F) < 10)
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;								
  else
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  
  //优先级
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = prior;  
  //子优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_prior;							
  //使能外部中断通道
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器//
  NVIC_Init(&NVIC_InitStructure);
  
}
