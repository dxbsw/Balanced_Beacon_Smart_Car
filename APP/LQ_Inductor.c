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

使用说明：
本教学演示程序适用于电磁四轮或者三轮车：
整车资源为：
模块：龙邱TC264DA核心板、配套母板、双路全桥电机驱动、双编码器512带方向、TFT1.8屏幕、单舵机、四路电感模块；
车模：三轮或者四轮均可；
电感分布：
||----------左------------------------------------右--------------||
||---------侧--------------------------------------侧-------------||
||--------第----------------------------------------第------------||
||-------1----左侧第2个电感 -------右侧第2个电感 -----1-----------||
||------个--------------------------------------------个----------||
||-----电----------------------------------------------电---------||
||----感------------------------------------------------感--------||
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

int16_t TempAngle = 0;        // 根据电感偏移量计算出的临时打角值

int16_t Offset = 0;      // 根据电感偏移量计算出的临时打角值
int16_t LastAngle = 0;        // 记忆冲出赛道前的有效偏移方向
// Front Back Vertical Parallel
int16_t LleftP = 0, LleftV = 0, LrightV = 0, LrightP = 0;  // 电感偏移量
int16_t LnowADC[4];           // 电感当前ADC数值

int16_t ad_max[4] = {2500, 2300, 999, 999}; // 新板子放到赛道标定最大值,会被程序刷新
int16_t ad_min[4] = { 120,  120, 120, 120}; // 新板子据需要标定最小值,会被程序刷新

u8 CircleNumber = 1;    // 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
u8 TangentPoint = 1;    // 切点判断   0切点结束；默认1可以入环，读取脉冲为入环准备
u8 EnterCircle = 0;     // 允许进环  默认 0不可进环；1可以进环
u8 OutCircle = 0;       // 允许出环   默认0不可出环；1可以出环
u8 LeftRightCircle = 0; // 左侧环还是右侧环 默认0原始；1左环；2右环

int32_t TangentPointpulse = 0; // 脉冲记忆临时变量1
int32_t EnterCirclePulse = 0;  // 脉冲记忆临时变量2
int32_t OutCirclePulse = 0;    // 脉冲记忆临时变量3
int32_t EnterCircleOKPulse = 0;// 脉冲记忆临时变量4
int16_t LowSpeed = 0;          // 圆环/十字口减速
u8 Flag_Show=0;
u16 MagneticField = 0;     // 磁场强度    magnetic field intensity,判断圆环是否出现
int16_t OffsetDelta = 0;
extern pid_param_t pid1;


/*************************************************************************
*  函数名称：void InductorInit (void)
*  功能说明：四个电感ADC初始化函数；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void InductorInit (void)
{
  ADC1Init(ADC_Channel_4);
  ADC1Init(ADC_Channel_5);
  ADC1Init(ADC_Channel_6);
  ADC2Init(ADC_Channel_7);	
}
/*************************************************************************
*  函数名称：void InductorNormal(void)
*  功能说明：采集电感电压并归一化；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：   注意要先标定运放的放大倍数，尽量四个一致或者接近
*************************************************************************/
void InductorNormal (void)
{
  LnowADC[0] = ADC1_Read(ADC_Channel_4);   // 左前侧第1个电感，与赛道平行，采集各个电感的AD值
  LnowADC[1] = ADC1_Read(ADC_Channel_5);   // 左前侧第2个电感，与垂直赛道，
  LnowADC[2] = ADC1_Read(ADC_Channel_6);  // 右前侧第2个电感，与垂直赛道，
  LnowADC[3] = ADC2_Read(ADC_Channel_7);  // 右前侧第1个电感，与赛道平行，
  
  if (LnowADC[0] < ad_min[0])       
    ad_min[0] = LnowADC[0];    // 刷新最小值
  else if (LnowADC[0] > ad_max[0])  
    ad_max[0] = LnowADC[0];    // 刷新最大值
  
  if (LnowADC[1] < ad_min[1])        
    ad_min[1] = LnowADC[1];
  else if (LnowADC[1] > ad_max[1])   
    ad_max[1] = LnowADC[1];
  
  if (LnowADC[2] < ad_min[2])       
    ad_min[2] = LnowADC[2];
  else if (LnowADC[2] > ad_max[2])  
    ad_max[2] = LnowADC[2];
  
  if (LnowADC[3] < ad_min[3])      
    ad_min[3] = LnowADC[3];
  else if (LnowADC[3] > ad_max[3])  
    ad_max[3] = LnowADC[3];
  
  LleftP  = (LnowADC[0] - ad_min[0]) * 100 / (ad_max[0] - ad_min[0]);     // 各偏移量归一化到0--100以内
  LleftV  = (LnowADC[1] - ad_min[1]) * 100 / (ad_max[1] - ad_min[1]);
  LrightV = (LnowADC[2] - ad_min[2]) * 100 / (ad_max[2] - ad_min[2]);
  LrightP = (LnowADC[3] - ad_min[3]) * 100 / (ad_max[3] - ad_min[3]);
  

}


/*************************************************************************
*  函数名称：void TFT_Show_EleMag_Info(void)
*  功能说明：显示各种所需信息
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void TFT_Show_EleMag_Info(void)
{ 
  char txt[16] = "X:"; 
  //显示各电感归一化后的偏移量  当前各电感电压值 各电感开机后历史最小值 各电感开机后历史最大值
  sprintf(txt, "0:%04d %04d  ", LleftP, LnowADC[0]);
  TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
  sprintf(txt, "1:%04d %04d", LleftV, LnowADC[1]);
  TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
  sprintf(txt, "2:%04d %04d  ", LrightV, LnowADC[2]);
  TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
  sprintf(txt, "3:%04d %04d  ", LrightP, LnowADC[3]);
  TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK); 
  sprintf(txt, "4:%04d %04d  ", ECPULSE1, ECPULSE2);
  TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK); 
  sprintf(txt, "5:%04d ", MagneticField );
  TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);   
}
/*************************************************************************
*  函数名称：void ElectroMagneticCar(void)
*  功能说明：电磁车双电机差速控制
-->1.入门算法：简单的分段比例控制算法，教学演示控制算法；
---2.进阶算法：PID典型应用控制算法，教学演示控制算法；
---3.高端算法：暂定改进粒子群协同控制算法；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年10月28日
*  备    注：驱动2个电机
*************************************************************************/
void ElectroMagneticMWCar (void)
{
  InductorInit();       // 电感器ADC初始化    
  Encoder_Init(TIM3_ENCA_B4,TIM3_ENCB_B5);// 初始化两个编码器
  Encoder_Init(TIM4_ENCA_B6,TIM4_ENCB_B7); 	
  MotorInit(12500);     // 电机驱动的PWM为12.5Khz
  TIMER_InitConfig(TIMER_6, 20);//编码器读取中断函数
 
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLACK);         // 清屏
  while (1)
  {
    InductorNormal();           // 采集电感电压并并归一化；
    MagneticField = LleftP + LleftV + LrightV + LrightP;// 磁场整体强度
    if((LleftV>28)&&(LrightV>28))
    {
      Offset = (LleftV-LrightV)*20;
    }
    else 
    {
      Offset = (LleftV-LrightV)*35;
    }
   // TFT_Show_EleMag_Info();
    MotorDuty1 = MtTargetDuty - Offset*8/5- ECPULSE1 * 3;        // 电机PWM
    MotorDuty2 = MtTargetDuty + Offset*8/5+ ECPULSE2 * 3;        // 双电机差分，需要去掉abs
    
    MotorCtrl(MotorDuty1, MotorDuty2);        // 三轮双电机驱动
  }
} 

