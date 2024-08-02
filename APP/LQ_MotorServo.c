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

uint16_t BatVolt;         // 电池电压采集
volatile uint8_t Game_Over = 0; // 小车完成全部任务，停车
int16_t ServoDuty = Servo_Center_Mid;
int MotorDuty1 = 500;      // 电机驱动占空比数值
int MotorDuty2 = 500;      // 电机驱动占空比数值
int32_t NowTime = 0;
uint16_t BatVolt = 0;           // 电池电压采集

// 四个轮子的期望速度
//signed short targetSpeed1 = 0, targetSpeed2 = 0, targetSpeed3 = 0, targetSpeed4 = 0;
// 四个轮子的实际速度，另外一个核心板需要提供两路编码器速度
signed short  realSpeed1 = 0, realSpeed2 = 0, realSpeed3 = 0, realSpeed4 = 0;
pid_param_t pid1, pid2, pid3, pid4;
// 期望速度
volatile int16_t targetSpeed = 0;
//电机频率
#define MOTOR_FREQUENCY    PWM_DUTY_MAX
extern TIM_TypeDef *tim_index[7];

//#define USE7843or7971   //USEDRV8701 使用龙邱不同的驱动模块，选择对应的宏定义
#define USEDRV8701

/*************************************************************************
*  函数名称：void MotorInit(u16 freq)
*  功能说明：电机PWM初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个电机  TIM8_PWM_Init(1000-1);  //1KHZ周期
*************************************************************************/
void MotorInit(u16 freq)
{
  PIN_InitConfig(PC6, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC7, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC8, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PC9, GPO, 1,GPIO_Mode_Out_PP);
  
  PIN_InitConfig(PG6, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PG7, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PG8, GPO, 1,GPIO_Mode_Out_PP);
  PIN_InitConfig(PG5, GPO, 1,GPIO_Mode_Out_PP);
  
  PWM_InitConfig(PWM8_CH1_PC6,freq, 0);
  PWM_InitConfig(PWM8_CH3_PC8,freq, 0);
  
  PidInit(&pid1);
  PidInit(&pid2);
  PidInit(&pid3);
  PidInit(&pid4);
  /* PID 参数需要自行调节 */
  pid1.kp = 150;
  pid1.ki = 5;
  
  pid2.kp = 150;
  pid2.ki = 5;
  
  pid3.kp = 150;
  pid3.ki = 5;
  
  pid4.kp = 150;
  pid4.ki = 5;
}

/*!
* @brief    速度控制函数
*
* @param    x   ： x方向速度                          小车正右方
* @param    y   ： y方向速度                          小车正前方
* @param    z   ： 逆时针方向速度
*
* @return   无
*
* @note     无
*
* @see      无
*
* @date     2020/6/8
*/
void SpeedCtrl(signed short x, signed short y, signed short z)
{
  // 根据小车速度大小和方向  计算每个轮子的期望转速

  
  
  // 获取编码器值
  realSpeed1 = Read_Encoder(4); //左电机 母板上编码器1，小车前进为负值
  realSpeed2 = Read_Encoder(2); //右电机 母板上编码器2，小车前进为正值
  //realSpeed3 = Read_Encoder(4); //另外一个核心板发数据过来
  //realSpeed4 = Read_Encoder(2); //另外一个核心板发数据过来
  
  // PID处理

  
  // 电机控制
  //MotorCtrl4w(pid1.out, pid2.out, pid3.out, pid4.out);
  
  // 发送数据到匿名上位机画波形   方便调试PID参数
  //  ANO_DT_send_int16(targetSpeed1, realSpeed1, (signed short)(pid1.out), 0, 0, 0, 0, 0);
  
}


/*************************************************************************
*  函数名称：void MotorCtrl(int16_t motor1, int16_t motor2)
*  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
*  参数说明：
*    @param    motor1   ： 电机1占空比，范围【-10000，10000】
@param    motor2   ： 电机2占空比，范围【-10000，10000】
@param    motor3   ： 电机3占空比，范围【-10000，10000】
@param    motor4   ： 电机4占空比，范围【-10000，10000】
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
*************************************************************************/
#ifdef USE7843or7971
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4)
{
  //assert(motor1>TIM3_PWM_DUTY_MAX);
  //assert(motor2>TIM3_PWM_DUTY_MAX);
  //assert(motor3>TIM3_PWM_DUTY_MAX);
  //assert(motor4>TIM3_PWM_DUTY_MAX);
  
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM8_CH1_PC6,motor1);	     //C6
    GPIO_ResetBits(MTGPIO1p);    //G5
  }
  else
  {
    PWM_Set_Duty(PWM8_CH1_PC6,MOTOR_FREQUENCY+motor1);
    GPIO_SetBits(MTGPIO1p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM8_CH2_PC7,motor2);	    //C7
    GPIO_ResetBits(MTGPIO2p);   //G6	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH2_PC7,MOTOR_FREQUENCY+motor2);
    GPIO_SetBits(MTGPIO2p);
  }
  
  if (motor3 > 0)
  {
    PWM_Set_Duty(PWM8_CH3_PC8,motor3);	  //C8
    GPIO_ResetBits(MTGPIO3p); //G7	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH3_PC8,MOTOR_FREQUENCY+motor3);
    GPIO_SetBits(MTGPIO3p);
  }
  
  if (motor4 > 0)
  {
    PWM_Set_Duty(PWM8_CH4_PC9,motor4);	 //C9
    GPIO_ResetBits(MTGPIO4p);//C8	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH4_PC9,MOTOR_FREQUENCY+motor4);
    GPIO_SetBits(MTGPIO4p);
  }
}
void MotorCtrl(int16_t motor1, int16_t motor2)
{
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM8_CH1_PC6,motor1);	     //C6
    GPIO_ResetBits(MTGPIO1p);    //G5
  }
  else
  {
    PWM_Set_Duty(PWM8_CH1_PC6,MOTOR_FREQUENCY+motor1);
    GPIO_SetBits(MTGPIO1p);
  }
  
  if (motor2 > 0)
  {
    PWM_Set_Duty(PWM8_CH2_PC7,motor2);	    //C7
    GPIO_ResetBits(MTGPIO2p);   //G6	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH2_PC7,MOTOR_FREQUENCY+motor2);
    GPIO_SetBits(MTGPIO2p);
  }
  

}
#else   //USEDRV8701    
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4)
{  
  if (motor1 > 0)
  {
    PWM_Set_Duty(PWM8_CH1_PC6,motor1);	    //C6
    GPIO_ResetBits(MTGPIO1p);   //G5	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH1_PC6,0-motor1);
    GPIO_SetBits(MTGPIO1p);
  }
  
 if (motor2 > 0)
  {
    PWM_Set_Duty(PWM8_CH2_PC7,motor2);	 //C7
    GPIO_ResetBits(MTGPIO2p);//G6	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH2_PC7,0-motor2);
    GPIO_SetBits(MTGPIO2p);
  }
  
    if (motor3 > 0)
  {
    PWM_Set_Duty(PWM8_CH3_PC8,motor3);	     //C8
    GPIO_ResetBits(MTGPIO3p);    //G7
  }
  else
  {
    PWM_Set_Duty(PWM8_CH3_PC8,0-motor3);
    GPIO_SetBits(MTGPIO3p);
  }
  if (motor4 > 0)
  {
    PWM_Set_Duty(PWM8_CH4_PC9,motor4);	  //C9
    GPIO_ResetBits(MTGPIO4p); //G8	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH4_PC9,0-motor4);
    GPIO_SetBits(MTGPIO4p);
  }  

}

void MotorCtrl(int16_t motor1, int16_t motor2)
{
    if (motor1 > 0)
  {
    PWM_Set_Duty(PWM8_CH3_PC8,motor1);	     //C8
    GPIO_ResetBits(MTGPIO3p);    //G7
  }
  else
  {
    PWM_Set_Duty(PWM8_CH3_PC8,0-motor1);
    GPIO_SetBits(MTGPIO3p);
  }
   if (motor2 > 0)
  {
    PWM_Set_Duty(PWM8_CH4_PC9,motor2);	  //C9
    GPIO_ResetBits(MTGPIO4p); //G8	
  }
  else
  {
    PWM_Set_Duty(PWM8_CH4_PC9,0-motor2);
    GPIO_SetBits(MTGPIO4p);
  }
}
#endif

/*************************************************************************
*  函数名称：BalMotorCtrl(int16_t motor1, int16_t motor2)
*  功能说明：直立车模BTN驱动专用电机输出函数
*  参数说明：无
*  函数返回：无
*  修改时间：2021年4月2日
*  备    注：驱动2个电机
【注意事项】驱动能力与以上函数不同，BTN驱动直立专用!
*************************************************************************/
#define Death_Duty_motor1 350 //定义死区电压
#define Death_Duty_motor2 280 //定义死区电压
void BalMotorCtrl(int16_t motor1, int16_t motor2)
{
  if (motor1 > 0)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_7);
    PWM_Set_Duty(PWM8_CH1_PC6,motor1+Death_Duty_motor1);	    //C6
   // PWM_Set_Duty(PWM8_CH2_PC7,0);	                    //C7
  }
  else if (motor1 < 0)
  {
    GPIO_ResetBits(GPIOC,GPIO_Pin_7);    
    PWM_Set_Duty(PWM8_CH1_PC6,0-motor1+Death_Duty_motor1);	                    //C6
   // PWM_Set_Duty(PWM8_CH2_PC7,0-motor1+Death_Duty);	    //C7
  }
  else
  {
    PWM_Set_Duty(PWM8_CH1_PC6,0);	                    //C6
   // PWM_Set_Duty(PWM8_CH2_PC7,0);	                    //C7
  }
    
   if (motor2 > 0)
  {
    GPIO_ResetBits(GPIOC,GPIO_Pin_9);    
    PWM_Set_Duty(PWM8_CH3_PC8,motor2+Death_Duty_motor2);	    //C8
   // PWM_Set_Duty(PWM8_CH4_PC9,0);	                    //C9	
  }
  else if (motor2 < 0)
  {
    GPIO_SetBits(GPIOC,GPIO_Pin_9);    
    PWM_Set_Duty(PWM8_CH3_PC8,0-motor2+Death_Duty_motor2);	                    //C8
   // PWM_Set_Duty(PWM8_CH4_PC9,0-motor2+Death_Duty);	    //C9
  }
  else
  {
    PWM_Set_Duty(PWM8_CH3_PC8,0);	                    //C6
    //PWM_Set_Duty(PWM8_CH4_PC9,0);	                    //C7
  }
}
/*************************************************************************
*  函数名称：TestMotor(void)
*  功能说明：测试标定输出PWM控制电机
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个电机
【注意事项】注意，一定要对舵机打角进行限制
使用龙邱母板测试流程：
1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
2.接好母板到驱动板的信号线及电源线；
3.接好驱动板到电机的导线；
4.烧写程序并运行，确定电机能正常转动后，开启驱动板电源开关；
5.按键K0/K1确定电机转动速度及方向；
6.如果出现疯转，按下K2键返回低速模式，或者直接关闭驱动板电源！
*************************************************************************/
void TestMotor (void)
{
  int16_t duty = 1500;
  char txt[16];
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);          //清屏
  TFTSPI_P8X16Str(2, 0, "LQ Motor Test", u16RED, u16BLUE);
  MotorInit(12500);
  while (1)
  {
    if (KEY_Read(KEY0) == 0)      //按下KEY0键，占空比减小
    {
      if (duty > -PWM_DUTY_MAX)
        duty -= 500;
    }
    if (KEY_Read(KEY2) == 0)      //按下KEY2键，占空比加大
    {
      if (duty < PWM_DUTY_MAX) //满占空比为10000
        duty += 500;
    }
    if (KEY_Read(KEY1) == 0)      //按下KEY1键，占空比中值
    {
      duty = 1500;
    }
    
    // MotorCtrl(duty, duty);
    MotorCtrl4w(duty, duty, duty, duty);
    
    sprintf(txt, "PWM: %05d;", duty);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);       //字符串显示   
    
    LED_Ctrl(LED1, RVS);       //电平翻转,LED闪烁
    delayms(200);              //延时等待
  }
}
/*************************************************************************
*  函数名称：void ServoInit(void)
*  功能说明：舵机PWM初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机 TIM4_PWM_Init(100-1);  //100HZ周期
*************************************************************************/
void ServoInit(u16 freq)
{
  PWM_InitConfig(PWM5_CH1_PA0, 100, 1500);  //舵机默认用的是A0口，TIM5定时器
  PWM_InitConfig(PWM5_CH2_PA1, 100, 1500);  //舵机默认用的是A1口，TIM5定时器
}

/*************************************************************************
*  函数名称：void ServoCtrl(uint32_t duty)
*  功能说明：舵机转角函数，由于小车拉杆范围限制，较小
*  参数说明：short duty，占空比
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机，普通四轮只需要一个舵机即可
*************************************************************************/
void ServoCtrl (uint16_t duty)
{
  /*
  if (duty >= Servo_Left_Max)                  //限制幅值
  duty = Servo_Left_Max;
  else if (duty <= Servo_Right_Min)            //限制幅值
  duty = Servo_Right_Min;
  */
  PWM_Set_Duty(PWM5_CH1_PA0,duty);//A0;	
  PWM_Set_Duty(PWM5_CH2_PA1,duty); //　A1
}

/*************************************************************************
*  函数名称：Test_Servo(void)
*  功能说明：舵机PWM初始化，测试标定输出PWM控制SD5/S3010舵机
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机
【注意事项】注意，一定要对舵机打角进行限制
使用龙邱母板测试流程：
1.先使用万用表测量电池电压，务必保证电池电压在7V以上，否则无力不反应！
2.然后确定舵机供电电压，SD5舵机用5V供电，S3010用6-7V供电！！！
3.把舵机的舵盘去掉，让舵机可以自由转动；
4.烧写程序并运行，让舵机转动到中值附近；如果没反应重复1-2步，或者调整舵机的PWM频率计占空比，能受控为准；
5.舵机受控后用手轻转，舵机会吱吱响，对抗转动，此时可以装上舵盘；
6.按键K0/K1确定舵机的左右转动极限，并记下来，作为后续限幅防止舵机堵转烧毁！
*************************************************************************/
void TestServo (void)
{
  char txt[16] = "X:";
  signed short duty = Servo_Center_Mid;
  
  TFTSPI_CLS(u16BLUE);          //清屏
  TFTSPI_P8X16Str(2, 0, "LQ Servo Test", u16RED, u16BLUE);
  ServoInit(100);
  ServoCtrl(Servo_Center_Mid);  //中值
  while (1)
  {
    if (!KEY_Read(KEY0))
    {
      if (duty > 100)  //防止duty超
      {
        duty -= 100;
      }
    }
    if (!KEY_Read(KEY1))
    {
      duty = Servo_Center_Mid;
    }
    if (!KEY_Read(KEY2))
    {
      duty += 100;
    }
    ServoCtrl(duty); 
    sprintf(txt, "Servo duty:%04d ", duty);
    TFTSPI_P8X16Str(1, 2, txt, u16BLACK, u16GREEN); //显示出库实际脉冲数，以便灵活调整

  }
}


/*************************************************************************
*  函数名称：uint8 SetCircleNum (void)
*  功能说明：设置需要进入圆环的个数；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
uint8_t SetCircleNum (void)
{
  char txt[16] = " ";
  uint8_t num = 1;
  
  TFTSPI_CLS(u16BLACK);            // 清屏
  TFTSPI_P8X16Str(2, 1, "LQ Smart Car", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 3, "K2 num +", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 4, "K1 set OK", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 5, "K0 num -", u16RED, u16BLUE);
  TFTSPI_P8X16Str(2, 7, "Ring num:  ", u16RED, u16BLUE);
  
  while (KEY_Read(KEY1))
  {
    if (KEY_Read(KEY2) == 0)
    {
      if (num < 10)
        num++;
    }
    else if (KEY_Read(KEY0) == 0)
    {
      if (num > 0)
        num--;
    }
    sprintf(txt, "Ring num: %d ", num);
    TFTSPI_P8X16Str(2, 7, txt, u16WHITE, u16BLUE);
    
    delayms(100);
  }
  return num;
}

/*************************************************************************
*  函数名称：void OutInGarage(uint8_t inout, uint8_t lr)
*  功能说明：出入库
*  参数说明：uint8 inout:0出库，1入库；
*          uint8_t lr：0左出入库；1右出入库
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：   OutInGarage(1,0); // 右侧出库
*************************************************************************/

void OutInGarage (uint8_t inout, uint8_t lr)
{
  int32_t ps = 0;
  
  if (lr)           // 1右出入库
  {
    if (inout)    // 1右入库
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // 获取STM0 当前时间，得到毫秒
      // 2020年新加出库元素，此处为盲走入库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(2000, 2000);        // 左后倒车
      while (RAllPulse < ps + 2000) // 继续前进大约35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // 刹车
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min);   // 舵机向右打死为出库做准备
      MotorCtrl(-3000, -2000);      // 右后倒车，左轮快，右轮慢，
      while (RAllPulse > ps - 2000) // 从停车位出库，大约要512编码器2000个脉冲，龙邱512带方向编码器1米5790个脉冲
      {   // 右电机 母板上编码器1，小车后退为正值，并累加到出库为止
        delayms(10);
      }
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(-2500, -2500);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 800)  // 右电机 母板上编码器1，小车后退为正值，并累加到出库为止
      {
        delayms(10);
      }
      //DisableInterrupts;  // 关闭所有中断
      MotorCtrl(3000, 3000);
      delayms(300);                // 电机反转刹车，防止滑出赛道，时间根据速度调整
      MotorCtrl(0, 0);             // 停车
      while (1);                   // 入库完毕，永久停车
    }
    else  // 0右出库
    {
      // 2020年新加出库元素，此处为盲走出库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // 直行大约10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Right_Min); // 舵机向右打死为出库做准备
      MotorCtrl(3500, 3000);       // 右转，左轮快，右轮慢，
      while (RAllPulse < ps + 1200)
      {
        delayms(10);
      }
    }
  }
  else // 0：左出入库；
  {
    if (inout) // 1左入库
    {
      //NowTime = (STM_GetNowUs(STM0) - NowTime) / 1000;  // 获取STM0 当前时间，得到毫秒
      // 2020年新加出库元素，此处为盲走入库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(2000, 2000);        // 左后倒车
      while (RAllPulse < ps + 2500) // 继续前进大约35cm
      {
        delayms(10);
      }
      MotorCtrl(-2500, -2500);      // 刹车
      delayms(300);
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max);    // 舵机向右打死为出库做准备
      MotorCtrl(-2000, -3000);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 3000)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid);  // 回中倒车
      MotorCtrl(-2500, -2500);      // 左后倒车，右轮快，左轮慢
      while (RAllPulse > ps - 1500)
      {
        delayms(10);
      }
      //DisableInterrupts; // 关闭所有中断
      MotorCtrl(3000, 3000);
      delayms(300);               // 电机反转刹车，防止滑出赛道，时间根据速度调整
      MotorCtrl(0, 0);            // 停车
      while (1) ;                 // 入库完毕，永久停车
    }
    else  // 0左出库
    {
      // 2020年新加出库元素，此处为盲走出库
      ps = RAllPulse;
      ServoCtrl(Servo_Center_Mid); // 直行大约10cm
      MotorCtrl(2500, 2500);       //
      while (RAllPulse < ps + 600)
      {
        delayms(10);
      }
      
      ps = RAllPulse;
      ServoCtrl(Servo_Left_Max); // 舵机向左打死为出库做准备
      MotorCtrl(3000, 3500);     // 左转，右轮快，左轮慢，
      while (RAllPulse < ps + 1800)
      {
        delayms(10);
      }
    }
  }
}

/*************************************************************************
*  函数名称：uint8 ReadOutInGarageMode(void)
*  功能说明：读取拨码开关设置出入库模式
*  参数说明：无
*  函数返回：出入库模式,0左出入库；默认1右出入库
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
uint8_t ReadOutInGarageMode (void)
{
  return (KEY_Read(DSW0));
}
//
