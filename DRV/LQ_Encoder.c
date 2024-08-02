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

#define ENCODER_TIM_PERIOD (u16)(65535)   //不可大于65535 因为F103的定时器是16位的。

/*************************************************************************
*  函数名称：int Read_Encoder(u8 TIMX)
*  功能说明：读取编码器计数
*  参数说明：u8 TIMX定时器
*  函数返回：脉冲数量
*  修改时间：2020年3月10日
*  备    注：ECPULSE1 = Read_Encoder(3); //左电机 母板上编码器3，小车前进为负值
*************************************************************************/
int Read_Encoder(u8 TIMX)
{
  int Encoder_TIM;    
  switch(TIMX)
  {
  case 1:  Encoder_TIM= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//摄像头用	
  case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;//编码器2用
  case 3:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;//编码器3用	
  case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;//编码器用
  case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;//舵机用	
  //case 6:  Encoder_TIM= (short)TIM6 -> CNT;  TIM6 -> CNT=0;break;//定时器用
  //case 7:  Encoder_TIM= (short)TIM7 -> CNT;  TIM7 -> CNT=0;break;//定时器用
  case 8:  Encoder_TIM= (short)TIM8 -> CNT;  TIM8 -> CNT=0;break;//电机用
  default:  Encoder_TIM=0;
  }
  return Encoder_TIM;
}

int Read_DirEncoder(u8 TIMX)
{
  int Encoder_TIM;    
  switch(TIMX)
  {
  case 1:  Encoder_TIM= (short)TIM1 -> CNT;  TIM1 -> CNT=0;break;//摄像头用	
  case 2: 
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)){ Encoder_TIM=-abs((short)TIM2 -> CNT);  TIM2 -> CNT=0;}
    else {Encoder_TIM=abs((short)TIM2 -> CNT);  TIM2 -> CNT=0;}
      break;//编码器2用
    
  case 3:  
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)){ Encoder_TIM=abs((short)TIM3 -> CNT);  TIM3 -> CNT=0;}
    else {Encoder_TIM=-abs((short)TIM3 -> CNT);  TIM3 -> CNT=0;}
      break;//编码器2用
    
  case 4:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;//编码器用
  case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;//舵机用	
  //case 6:  Encoder_TIM= (short)TIM6 -> CNT;  TIM6 -> CNT=0;break;//定时器用
  //case 7:  Encoder_TIM= (short)TIM7 -> CNT;  TIM7 -> CNT=0;break;//定时器用
  case 8:  Encoder_TIM= (short)TIM8 -> CNT;  TIM8 -> CNT=0;break;//电机用
  default:  Encoder_TIM=0;
  }
  return Encoder_TIM;
}

/*************************************************************************
*  函数名称：void Encoder_Init(ENCODER_PIN_enum ap,ENCODER_PIN_enum bp)
*  功能说明：编码器初始化
*  参数说明：ENCODER_PIN_enum ap,通道A所对应的的定时器及管脚
             ENCODER_PIN_enum bp,通道B所对应的的定时器及管脚
*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：Encoder_Init(TIM4_ENCA_B06,TIM4_ENCB_B07);//两者要成对出现
*************************************************************************/
void Encoder_Init(ENCODER_PIN_enum ap,ENCODER_PIN_enum bp)
{  
  unsigned char timern =(ap & 0xF000) >>12;  
  
  PIN_AFIO_Init((GPIO_Name_t)(ap &0xFF), GPI, GPI_FL,(GPIO_AF_t)((ap &0xF00)>>8));//引脚复用初始化
  PIN_AFIO_Init((GPIO_Name_t)(bp &0xFF), GPI, GPI_FL,(GPIO_AF_t)((bp &0xF00)>>8));//引脚复用初始化

  if(timern==0) 
  {    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
  }
  else if(timern==7) 
  {   
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM8, ENABLE);      
  }
  else
  {
    //RCC->APB1ENR |= ((uint32_t)0x00000001 << (timern-1));// 开启TIM2345时钟      
    RCC_APB1PeriphClockCmd(0x01U << (timern-1), ENABLE);      
  }

  TIMERxP[timern]->ARR = 0xFFFF;	            //设定计数器自动重装值
  TIMERxP[timern]->PSC = 0;		            //预分频器  
  TIMERxP[timern]->CCMR1 |= TIM_CCMR1_CC1S_DIRECTTI;// 输入捕获IC1映射到TI1
  TIMERxP[timern]->CCMR1 |= TIM_CCMR1_CC2S_DIRECTTI;// 输入捕获IC2映射到TI2  
  TIMERxP[timern]->SMCR |= TIM_SMCR_SMS_ENCODER3;   // 使用编码器模式为TI12支持带龙-邱方向的编码器 ,分别可以选择1，2，3 模式
  TIMERxP[timern]->CR1 |= TIM_CR1_UDIS;		    // 禁止UEV事件的产生
  TIMERxP[timern]->CR1 |= TIM_CR1_CEN;              // 定时器使能
}


/*************************************************************************
*  函数名称：void EncInit (void)
*  功能说明：编码器初始化函数，传统结构体方式
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动4个编码器
*************************************************************************/
void EncInit (void)
{    
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_2);//管脚复用AF2
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_2);//管脚复用AF2
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_2);//管脚复用AF2  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;//PB4,6,7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_Init( GPIOB, &GPIO_InitStructure); 
  
  //编码器3
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);       //初始化结构体
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式为TI12支持带龙-邱方向的编码器
  TIM_Cmd(TIM3, ENABLE);                                //定时器使能
    
  //编码器4  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);       //初始化结构体
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式为TI12支持带龙-邱方向的编码器
  TIM_Cmd(TIM4, ENABLE);                                //定时器使能  
	
  //编码器2
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_1);//管脚复用AF1
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_1); //管脚复用AF1
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //PA15
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//输入，悬空
  GPIO_Init( GPIOA, &GPIO_InitStructure);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE );
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 1;                                        //预分频器
  TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                                      //设定计数器自动重装值
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                         //选择时钟分频：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;                     //TIM向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                                //重复计数器
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);       //初始化结构体
  TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12 , TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式为TI12支持带龙-邱方向的编码器
  TIM_Cmd(TIM2, ENABLE);   //定时器使能
  //方向引脚初始化
  PIN_InitConfig(PB5, GPI, 1,GPIO_Mode_IPU);
  PIN_InitConfig(PB3, GPI, 1,GPIO_Mode_IPU);
  
  
}


/*************************************************************************
*  函数名称：void TestEncoder(void)
*  功能说明：测试程序，TFT1.8显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void TestEncoderTFT(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(3, 2,"LQ encoder", u16PURPLE, u16BLACK); // 字符串显示
  //EncInit();
  Encoder_Init(TIM1_ENCA_E9, TIM1_ENCB_E11);
  Encoder_Init(TIM2_ENCA_A15,TIM2_ENCB_B3);
  Encoder_Init(TIM3_ENCA_B4, TIM3_ENCB_B5);
  Encoder_Init(TIM4_ENCA_B6, TIM4_ENCB_B7);  
  while (1)
  {
    /* 获取编码器值 */ 
    ECPULSE1 = Read_Encoder(1); //左电机 母板上编码器1，小车前进为负值
    ECPULSE2 = Read_Encoder(2); //右电机 母板上编码器2，小车前进为正值    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示    
    
    ECPULSE1 = Read_Encoder(3); //左电机 母板上编码器1，小车前进为负值
    ECPULSE2 = Read_Encoder(4); //右电机 母板上编码器2，小车前进为正值    
    sprintf(txt, "Enc3: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(3, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf(txt, "Enc4: %05d;", ECPULSE2);
    TFTSPI_P8X16Str(3, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示    
    UART_PutStr(UART1,txt); 
    LED_Ctrl(LED0, RVS);        //电平翻转,LED闪烁
    delayms(200);               //延时等待
  }
}

void TestEncoderTimerTFT(void)
{
  char txt[32];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(3, 2,"LQ encoder", u16PURPLE, u16BLACK); // 字符串显示
  Encoder_Init(TIM1_ENCA_E9, TIM1_ENCB_E11);
  Encoder_Init(TIM2_ENCA_A15,TIM2_ENCB_B3);
  Encoder_Init(TIM3_ENCA_B4, TIM3_ENCB_B5);
  Encoder_Init(TIM4_ENCA_B6, TIM4_ENCB_B7); 
  TIMER_InitConfig(TIMER_6, 20);//编码器读取中断函数
  while (1)
  {   
    sprintf(txt, "Enc3: %05d;", ECPULSE1);
    TFTSPI_P8X16Str(3, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
    UART_PutStr(UART1,txt); 
    
    sprintf(txt, "Enc4: %05d;", ECPULSE2);    
    TFTSPI_P8X16Str(3, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示    
    UART_PutStr(UART1,txt); 
    LED_Ctrl(LED0, RVS);        //电平翻转,LED闪烁
    delayms(200);               //延时等待
  }
}















/*************************************************************************
*  函数名称：void TestEncoder(void)
*  功能说明：测试程序，OLED显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void TestEncoderOLED(void)
{
  char txt[32];
  OLED_Init();
  OLED_CLS();   //清屏
  OLED_P6x8Str(0, 0, "Test Encoder");      //字符串显示
  EncInit();
  while (1)
  {
    /* 获取编码器值 */
    ECPULSE1 = Read_Encoder(3); //左电机 母板上编码器1，小车前进为负值
    ECPULSE2 = Read_Encoder(4); //右电机 母板上编码器2，小车前进为正值
    
    sprintf(txt, "Enc1: %05d;", ECPULSE1);
    OLED_P6x8Str(0, 3, txt);   //字符串显示
    sprintf(txt, "Enc2: %05d;", ECPULSE2);
    OLED_P6x8Str(0, 5, txt);   //字符串显示
    
    LED_Ctrl(LED0, RVS);       //电平翻转,LED闪烁
    delayms(200);              //延时等待
  }
}



