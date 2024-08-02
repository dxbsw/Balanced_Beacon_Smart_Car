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

const u32 TIMERx[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE, TIM5_BASE, TIM6_BASE, TIM7_BASE, TIM8_BASE};
TIM_TypeDef *TIMERxP[8] = {TIM1,TIM2,TIM3,TIM4,TIM5,TIM6,TIM7,TIM8};

int16_t ECPULSE1 = 0;          // 速度全局变量
int16_t ECPULSE2 = 0;         // 速度全局变量
int16_t ECPULSE =0;
//int16_t last_ECPULSE = 0;
volatile int32_t RAllPulse = 0;// 速度全局变量
extern volatile uint8_t Game_Over;    // 小车完成全部任务，停车
//extern volatile int16_t targetSpeed;


short aacx, aacy, aacz;
short gyrox, gyroy, gyroz;

//直立环参数
pid_param_t Gyro_PID,Angle_PID;
extern short gyro[3];
extern float Roll;
float ANGLE_OUT;
float BALANCE_OUT;

//速度环参数
pid_param_t BalSpeed_PID;
float SPEED_OUT;

//方向环参数
pid_param_t BalDirgyro_PID,BalDir_PID;
extern int16_t LleftP, LleftV, LrightV, LrightP;   // 电感偏移量
float BalDirgyro_out;
float Dir_Error;
float last_error;
float last_hang;
float BalDir_out;
extern int bell_flag;

#define Angle_Set -36.9  // 根据陀螺仪安装角度矫正
#define Gyroy_Set 20     // 陀螺仪零点漂移
#define Gyroz_Set -7    // 陀螺仪零点漂移
#define PI 3.14159265

float *ANGLE;

//此例程只利用了一个中断，通过设置标志位实现不同时间间隔的时序控制，只供参考！！！
//此例程只利用了一个中断，通过设置标志位实现不同时间间隔的时序控制，只供参考！！！
//此例程只利用了一个中断，通过设置标志位实现不同时间间隔的时序控制，只供参考！！！
int count_LED = 0;
int count_5ms = 0;
int count_10ms = 0;
int count_20ms = 0;
void TIM1_UP_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源  
    
  }  
}
void TIM2_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    
  }  
}
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    
  }  
}
void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    
  }  
}


void TIM5_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED0,RVS);//LED翻转闪烁     
  }  
}

extern int16_t targetSpeed1 =60;  // 直立车前进目标

void TIM6_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
      
    LED_Ctrl(LED1,RVS);//LED翻转闪烁 
    ECPULSE1 = Read_DirEncoder(3);                                                   //左电机编码器3
    ECPULSE2 = Read_DirEncoder(2);                                                   //右电机编码器2
    if(count_5ms == 0)
    {
      ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);                      //20689
      LQ_DMP_Read(); 
      
      
      BALANCE_OUT = PidLocCtrl(&Gyro_PID, ANGLE_OUT - (float)(gyroy-Gyroy_Set));           //角速度环输出
      
      BalDirgyro_out = PidLocCtrl(&BalDirgyro_PID, BalDir_out - (float)(gyroz-Gyroz_Set)); //方向内环输出
    }
    if(count_10ms == 0)
    {
      if(Roll>-5) Roll=-1 ;//最小角度限副，避免发生贴地
      ANGLE = &Roll; //角度采值
      

      if(((ECPULSE1+ECPULSE2)>>2)>=(targetSpeed1-2)&&((ECPULSE1+ECPULSE2)>>2)<=(targetSpeed1+1)&&bell_flag) GPIO_ResetBits(GPIOD,GPIO_Pin_6);
      else GPIO_SetBits(GPIOD,GPIO_Pin_6);
      
      kongzhi_2();     //1为匀速控制    2为变速控制
      
     ANGLE_OUT = PidLocCtrl(&Angle_PID, *ANGLE - Angle_Set - SPEED_OUT);            //角度环输出
      
      BalDir_out = PidLocCtrl(&BalDir_PID, Dir_Error);                               //方向外环输出
    }
    
/**************************************************
普通增量式速度控制pid
***************************************************/    
//    if(count_20ms == 0)
//    {
//     ECPULSE = ECPULSE1+ECPULSE2;
//     SPEED_OUT = PidIncCtrl(&BalSpeed_PID, targetSpeed1 - (int)(ECPULSE/4)); //速度环输出
//     SPEED_OUT = constrain_float(SPEED_OUT, -20, 20);                               //速度环限幅
//     //BalSpeed_PID.out = constrain_float(BalSpeed_PID.out, -100, 100);                 //速度环限幅
//    }
/**************************************************
变积分pid速度控制，能用但不完善，需要改进
***************************************************/
    if(count_20ms == 0)
    {
     ECPULSE = ECPULSE1+ECPULSE2;
     SPEED_OUT = PID_realize(targetSpeed1,(int)(ECPULSE/4)); //速度环输出
     SPEED_OUT = constrain_float(SPEED_OUT, -10, 10);                               //速度环限幅
     //BalSpeed_PID.out = constrain_float(BalSpeed_PID.out, -100, 100);                 //速度环限幅
    }
    

    //将直立环与方向环相加
    MotorDuty1 =  (int)(BALANCE_OUT + BalDirgyro_out);
    MotorDuty2 =  (int)(BALANCE_OUT - BalDirgyro_out);
    
    //电机占空比限幅
    if(MotorDuty1 > 8000)        MotorDuty1 = 8000;
    else if(MotorDuty1 < -8000) MotorDuty1 = -8000;
    if(MotorDuty2 > 8000)        MotorDuty2 = 8000;
    else if(MotorDuty2 < -8000) MotorDuty2 = -8000;
    BalMotorCtrl(MotorDuty1, MotorDuty2);
    
    count_5ms++;
    count_10ms++;
    count_20ms++;
    if(count_5ms  == 1)count_5ms  = 0;
    if(count_10ms == 2)count_10ms = 0;
    if(count_20ms == 4)count_20ms = 0;
    
    
    count_LED++;
    if(count_LED == 200)
    {
      LED_Ctrl(LED0,RVS);//LED翻转闪烁 直观检查中断时间是否溢出
      count_LED = 0;
    }  
  }  
}
void TIM7_IRQHandler(void)
{
  long sta = TIM1->SR;			// 读取中断状态
  TIM1->SR &= ~sta;					// 清除中断状态
  //用户程序    
  LED_Ctrl(LED1,RVS);//LED翻转闪烁
  
  
}
void TIM8_UP_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源
  {
    TIM_ClearITPendingBit(TIM8, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
    //用户程序    
    LED_Ctrl(LED1,RVS);//LED翻转闪烁
    
  }  
}

/*************************************************************************
*  函数名称：void TIMER_InitConfig(TIMER_Name_t timern, u16 nms)
*  功能说明：电机PWM初始化
*  参数说明：
//  @param      timern      定时器通道
//  @param      nms          定时周期pch,PWM通道所对应的的定时器及管脚

*  函数返回：void
*  修改时间：2020年3月10日
*  备    注：TIMER_InitConfig(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
*************************************************************************/
void TIMER_InitConfig(TIMER_Name_t timern, u16 nms)
{  
  u32 freq_div = 0;                                          //分频值
  u32 tmperiod;                                             //周期值
  
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  if(TIMER_1 == timern)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);                //时钟使能
  else if(TIMER_2 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  else if(TIMER_3 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    
  else if(TIMER_4 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    
  else if(TIMER_5 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM5, ENABLE);
  else if(TIMER_6 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM6, ENABLE);    
  else if(TIMER_7 == timern)
    RCC_APB1PeriphClockCmd(RCC_APB1ENR_TIM7, ENABLE);
  else if(TIMER_8 == timern)
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_TIM8, ENABLE);
  
  freq_div = SystemCoreClock/10000;                                       //多少分频 ,设置为10K ,100us计数一次                        
  if(freq_div<1) freq_div=1;
  tmperiod = nms*10;                                                      //周期值 nms*10*100us=nms
  if(tmperiod<1) tmperiod=1;
  
  //定时器初始化
  TIM_TimeBaseStructure.TIM_Period = tmperiod-1;                          //设置自动重装载寄存器周期nms*10*100us=nms
  TIM_TimeBaseStructure.TIM_Prescaler = freq_div-1;                       //设置为100us计数一次
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //向上计数模式
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
  TIM_TimeBaseInit((TIM_TypeDef *)TIMERx[timern], &TIM_TimeBaseStructure);//初始化TIMx时基
  
  TIM_ITConfig((TIM_TypeDef *)TIMERx[timern],TIM_IT_Update,ENABLE );      //使能TIM中断
  
  //中断优先级设置
  if(TIMER_1 == timern)   nvic_init(TIM1_UP_IRQn, 0, 2, ENABLE);
  else if(TIMER_2 == timern)   nvic_init(TIM2_IRQn, 0, 2, ENABLE);
  else if(TIMER_3 == timern)   nvic_init(TIM3_IRQn, 0, 2, ENABLE);
  else if(TIMER_4 == timern)   nvic_init(TIM4_IRQn, 0, 2, ENABLE);
  else if(TIMER_5 == timern)   nvic_init(TIM5_IRQn, 0, 2, ENABLE);
  else if(TIMER_6 == timern)   nvic_init(TIM6_IRQn, 0, 2, ENABLE);
  else if(TIMER_7 == timern)   nvic_init(TIM7_IRQn, 0, 2, ENABLE);
  else if(TIMER_8 == timern)   nvic_init(TIM8_UP_IRQn, 0, 2, ENABLE);
  
  TIM_Cmd((TIM_TypeDef *)TIMERx[timern], ENABLE);  //使能TIMx  
}



/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】 void Test_Timer56(void)
【功  能】 测试定时中断和LED灯闪烁
【参数值】 无
【参数值】 无
【返回值】 无 
【作  者】 L Q
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 Test_GPIO_LED()
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void Test_TimerPIT56(void)
{
  GPIO_LED_Init();
  TIMER_InitConfig(TIMER_5, 50);
  TIMER_InitConfig(TIMER_6, 25);
  while(1)
  {
    LED_Ctrl(LED2,RVS);     //LED翻转闪烁
    delayms(50);
  }
}


/***************************************************************
控制部分
****************************************************************/
void kongzhi_1(void)//最简单版的匀速控制
{
     if(dotcnt) // 发现信标灯
      {
        
        Dir_Error = -(sumlie-65);//93右翼 63
        if(Dir_Error>55)
           Dir_Error = 75;
        }
      else
      {
        
        Dir_Error = 65 ;
        
      }    
}

void kongzhi_2(void)
{
  
    if(dotcnt)
    {
        Dir_Error = -(sumlie-63);
        last_error = Dir_Error;   //上一个目标的位置
        last_hang = sumhang;
        
        
        //多段速度控制
        if(sumhang>0 && sumhang<=40 )
        {
            targetSpeed1 = 80;//************************
            GPIO_SetBits(GPIOD,GPIO_Pin_6);
        }    
        else if(sumhang>40 && sumhang<=55 )
        {
            targetSpeed1 = 80;//************************
            GPIO_SetBits(GPIOD,GPIO_Pin_6);
        }
        else if(sumhang>55 && sumhang<=65 )
        {
            targetSpeed1 = 80;//************************
            GPIO_ResetBits(GPIOD,GPIO_Pin_6);
        }
        else if(sumhang>65 && sumhang<=80 )
        {
            targetSpeed1 = 70;//************************
            GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//            if(abs(Dir_Error)>5)
//            {
//                Dir_Error = Dir_Error + 5;
//            }
        }
        else if(sumhang>80 && sumhang<=110 )
        {
            targetSpeed1 = 60;//************************
            GPIO_ResetBits(GPIOD,GPIO_Pin_6);
//            if(abs(Dir_Error)>3)
//            {
//                Dir_Error = Dir_Error + 3;
//            }
        }
        
        else if(sumhang>110 && sumhang<=120 )
        {
            Dir_Error = 50;
            targetSpeed1 = 70;//************************
        }
    }
    else if(!dotcnt)
    {
      dotcnt = 0;
      //此处三个判断为简单的记忆功能，当现在找不到灯是先对灯的上一个位置进行判断
      //可以解决因为出现在摄像头边缘因为转速过快而丢失目标的情况，可少转3/4圈
      //此处修改需谨慎，容易产生bug
      if(last_error > 10 )
      {
        Dir_Error = 50 ;
      }
      else if(last_error < -10 )
      {
        Dir_Error = -50 ;
        
      }
      else if(last_error>=-10 && last_error<=10)
      {
          Dir_Error = 50;
      }
      targetSpeed1 =60;
        //Dir_Error = 65 ;
    }
}






