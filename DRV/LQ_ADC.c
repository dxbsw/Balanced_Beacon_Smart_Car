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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
电感电压采集模块
推荐使用ADC4--11，共8路ADC，可以满足电磁车电感电压采集；
电源采集用ADC12。 
=================================================================
MM32F3277
**ADC1:               
**ADC1CH0 : A0      
**ADC1CH1 : A1
**ADC1CH2 : A2          
**ADC1CH3 : A3          
**ADC1CH4 : A4           
**ADC1CH5 : A5
**ADC1CH6 : A6
**ADC1CH7 : A7
**ADC1CH8 : B0
**ADC1CH9 : B1
**ADC1CH10: C0
**ADC1CH11: C1
**ADC1CH12: C2
**ADC1CH13: C3
**ADC1CH14TEMP	// 内部温度传感器
**ADC1CH15VS		// 内部参考电压
**ADC2:
**ADC2CH0 : A0      
**ADC2CH1 : A1
**ADC2CH2 : A2          
**ADC2CH3 : A3          
**ADC2CH4 : A4           
**ADC2CH5 : A5
**ADC2CH6 : A6
**ADC2CH7 : A7
**ADC2CH8 : B0
**ADC2CH9 : B1
**ADC2CH10: C0
**ADC2CH11: C1
**ADC2CH12: C2
**ADC2CH13: C3
**ADC2CH12: C4
**ADC2CH13: C5
**ADC3:
**ADC3CH0 : A0      
**ADC3CH1 : A1
**ADC3CH2 : A2          
**ADC3CH3 : A3          
**ADC3CH4 : F6           
**ADC3CH5 : F7
**ADC3CH6 : F8
**ADC3CH7 : F9
**ADC3CH8 : F10
**ADC3CH9 : B1
**ADC3CH10: C0
**ADC3CH11: C1
**ADC3CH12: C2
**ADC3CH13: C3
**ADC3CH12: C2
**ADC3CH13: C3
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "include.h"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void ADC1Init(ADCCHANNEL_TypeDef ADC_Channel_x)
【功  能】 ADC1初始化,配置ADC1连续转换模式
【参数值】 ADC_Channel_x , x为0~15
【参数值】 
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ADC1Init(ADCCHANNEL_TypeDef ADC_Channel_x)
{
  GPIO_InitTypeDef GPIO_InitStructure;    
  ADC_InitTypeDef  ADC_InitStructure;	
  ADC_StructInit(&ADC_InitStructure);    
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  
  if(ADC_Channel_x<8)
  {  
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
  }
  else if(ADC_Channel_x<10)
  {
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x-8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  }    
  else 
  {
    GPIO_InitStructure.GPIO_Pin  =   ADC_Channel_x-10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);            
  }  
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率
  //ADC prescale factor
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;//时钟分频
  //Set ADC mode to continuous conversion mode
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;//连续转换模式
  //AD data right-justified
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init(ADC1, &ADC_InitStructure);  
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_x,0, ADC_SampleTime_1_5Cycles);//采样周期
  
  ADC_Cmd(ADC1, ENABLE);  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void ADC2Init(ADCCHANNEL_TypeDef ADC_Channel_x)
【功  能】 ADC2初始化,配置ADC1连续转换模式
【参数值】 ADC_Channel_x , x为0~13
【参数值】 
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ADC2Init(ADCCHANNEL_TypeDef ADC_Channel_x)
{
  GPIO_InitTypeDef GPIO_InitStructure;    
  ADC_InitTypeDef  ADC_InitStructure;	
  ADC_StructInit(&ADC_InitStructure);    
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  
  if(ADC_Channel_x<8)
  {  
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
  }
  else if(ADC_Channel_x<10)
  {
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x-8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  }    
  else 
  {
    GPIO_InitStructure.GPIO_Pin  =   ADC_Channel_x-10;
    GPIO_Init(GPIOB, &GPIO_InitStructure);            
  }  
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率
  //ADC prescale factor
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;//时钟分频
  //Set ADC mode to continuous conversion mode
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;//连续转换模式
  //AD data right-justified
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init(ADC2, &ADC_InitStructure);  
  
  ADC_RegularChannelConfig(ADC2, ADC_Channel_x,0, ADC_SampleTime_1_5Cycles);//采样周期
  
  ADC_Cmd(ADC2, ENABLE);  
  ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void ADC3Init(ADCCHANNEL_TypeDef ADC_Channel_x)
【功  能】 ADC3初始化,配置ADC1连续转换模式
【参数值】 ADC_Channel_x , x为0~13
【参数值】 
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SPIM_CSHigh(SPI1)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void ADC3Init(ADCCHANNEL_TypeDef ADC_Channel_x)
{
  GPIO_InitTypeDef GPIO_InitStructure;    
  ADC_InitTypeDef  ADC_InitStructure;	
  ADC_StructInit(&ADC_InitStructure);    
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBENR_GPIOF, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  
  
  if(ADC_Channel_x<4)
  {  
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
  }
  else if(ADC_Channel_x<9)
  {
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x+2;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
  } 
  else if(ADC_Channel_x<10)
  {
    GPIO_InitStructure.GPIO_Pin  =  ADC_Channel_x-8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  } 	
  else 
  {
    GPIO_InitStructure.GPIO_Pin  =   ADC_Channel_x-10;
    GPIO_Init(GPIOC, &GPIO_InitStructure);            
  }  
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//分辨率
  //ADC prescale factor
  ADC_InitStructure.ADC_PRESCARE = ADC_PCLK2_PRESCARE_16;//时钟分频
  //Set ADC mode to continuous conversion mode
  ADC_InitStructure.ADC_Mode = ADC_Mode_Continuous_Scan;//连续转换模式
  //AD data right-justified
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init(ADC3, &ADC_InitStructure);  
  
  ADC_RegularChannelConfig(ADC3, ADC_Channel_x,0, ADC_SampleTime_1_5Cycles);//采样周期
  
  ADC_Cmd(ADC3, ENABLE);  
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC1_Read(uint8_t ADC_Channel_x)
【功  能】获取ADC1转换数据
【参数值】ADC_Channel_x , x为0~15
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC1_Read(uint8_t ADC_Channel_x)
{ 
  while(ADC_Channel_x!=(((ADC1->ADDATA) >>16) & 0xf)); //找到对应的通道 
  while(!(ADC1->ADSTA&ADC_FLAG_EOC));                  //等待转换完成  
  return (ADC1->ADDATA&0xfff);                         //读取数据
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC2_Read(uint8_t ADC_Channel_x)
【功  能】获取ADC2转换数据
【参数值】ADC_Channel_x , x为0~13
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC2_Read(uint8_t ADC_Channel_x)
{ 
  while(ADC_Channel_x!=(((ADC2->ADDATA) >>16) & 0xf)); //找到对应的通道 
  while(!(ADC2->ADSTA&ADC_FLAG_EOC));                  //等待转换完成  
  return (ADC2->ADDATA&0xfff);                         //读取数据
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC3_Read(uint8_t ADC_Channel_x)
【功  能】获取ADC3转换数据
【参数值】ADC_Channel_x , x为0~13
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC3_Read(uint8_t ADC_Channel_x)
{ 
  while(ADC_Channel_x!=(((ADC3->ADDATA) >>16) & 0xf)); //找到对应的通道 
  while(!(ADC3->ADSTA&ADC_FLAG_EOC));                  //等待转换完成  
  return (ADC3->ADDATA&0xfff);                         //读取数据
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC1_Read_Average(uint8_t ADC_Channel_x,uint8_t times)  
【功  能】获取几次ADC1采样值的平均值
【参数值】ADC_Channel_x , x为0~15
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read_Average(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC1_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC1_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC2_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
【功  能】获取几次ADC2采样值的平均值
【参数值】ADC_Channel_x , x为0~13
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read_Average(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC2_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC2_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 ADC3_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
【功  能】获取几次ADC3采样值的平均值
【参数值】ADC_Channel_x , x为0~13
【返回值】ADC读到的值
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 vbat=ADC1_Read_Average(ADC_Channel_12);   
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 ADC3_Read_Average(uint8_t ADC_Channel_x,uint8_t times)
{
  u32 temp_val=0;
  u8 t;
  u8 delay;
  for(t=0;t<times;t++)
  {
    temp_val+=ADC3_Read(ADC_Channel_x);
    for(delay=0;delay<100;delay++);
  }
  return temp_val/times;
} 

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_ADC(void)
【功  能】测试函数
【参数值】无
【返回值】无
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】 Test_ADC()；//DC悬空不为零，随机的
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_ADC(void)
{
  u16 vbat;
  char txt[16];
  
  ADC1Init(ADC_Channel_4);
  ADC1Init(ADC_Channel_5);
  ADC1Init(ADC_Channel_6);
  ADC2Init(ADC_Channel_7);
  
  ADC2Init(ADC_Channel_8);
  ADC2Init(ADC_Channel_9);
  ADC2Init(ADC_Channel_10);
  ADC2Init(ADC_Channel_11);
  ADC1Init(ADC_Channel_12);
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕	
  while(1)
  {
    vbat=ADC1_Read(ADC_Channel_12);   
    vbat = vbat * 11 / 25;  // x/4095*3.3*100*5.7   （5.7为 1K+4.7K 分压电阻分压比例）
    sprintf(txt, "Bat:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095*3
    TFTSPI_P8X16Str(3, 0, txt, u16PURPLE, u16BLACK);   // 字符串显示
    UART_PutStr(UART1,txt); 
    
    vbat=ADC1_Read(ADC_Channel_4);   
    sprintf(txt, "ch4:%d", vbat);  // *3.3/4095
    TFTSPI_P8X16Str(3, 1, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    
    vbat=ADC1_Read(ADC_Channel_5);   
    sprintf(txt, "ch5:%d", vbat);  
    TFTSPI_P8X16Str(3, 2, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    vbat=ADC1_Read(ADC_Channel_6);   
    sprintf(txt, "ch6:%d ", vbat);  // *3.3/4095
    TFTSPI_P8X16Str(3, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示

    vbat=ADC2_Read(ADC_Channel_7);   
    sprintf(txt, "ch7:%d ", vbat);  // *3.3/4095*3
    TFTSPI_P8X16Str(3, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    
    vbat=ADC2_Read(ADC_Channel_8);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch8:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    
    vbat=ADC2_Read(ADC_Channel_9);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch9%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    vbat=ADC2_Read(ADC_Channel_10);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch10:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示    
    
    vbat=ADC2_Read(ADC_Channel_11);   
    vbat = vbat * 2 / 25;  // x/4095*3.3*100
    sprintf(txt, "ch11:%d.%02dV ", vbat / 100, vbat % 100);  // *3.3/4095
    TFTSPI_P8X16Str(3, 8, txt, u16PURPLE, u16BLACK);   // 字符串显示

    LED_Ctrl(LED0,RVS); 
    delayms(100); 
  }
  
}

