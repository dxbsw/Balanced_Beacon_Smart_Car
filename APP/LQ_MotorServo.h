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
#ifndef SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_
#define SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_

#include "include.h"

#define  Kbat       2    // 电池电压对电机占空比的影响系数
#define  Kencoder   5    // 编码器速度对电机占空比的影响系数
#define  Koffset    1    // 赛道曲率对电机占空比的影响系数

#define TURN_LEFT    0   // 左转
#define TURN_RIGHT   1   // 右转
#define IN_GARAGE    1   // 入库
#define OUT_GARAGE   0   // 出库

#define Servo_Delta 150            //舵机左右转动的差值，与舵机型号，拉杆和舵机臂长有关
#define Servo_Center_Mid 1440      //舵机直行中值，
#define Servo_Left_Max   (Servo_Center_Mid+Servo_Delta)      //舵机左转极限值
#define Servo_Right_Min  (Servo_Center_Mid-Servo_Delta)      //舵机右转极限值，此值跟舵机放置方式有关，立式

#define MtTargetDuty  2000  //电机占空比值得最大值，实际需要减去电池电压，编码器，赛道曲率，达不到该值

#define MTGPIO1p      GPIOG,GPIO_Pin_5 
#define MTGPIO2p      GPIOG,GPIO_Pin_6   
#define MTGPIO3p      GPIOG,GPIO_Pin_7   
#define MTGPIO4p      GPIOG,GPIO_Pin_8  

extern int16_t ServoDuty;
extern int MotorDuty1;      // 电机驱动占空比数值
extern int MotorDuty2;      // 电机驱动占空比数值

extern uint16_t BatVolt;         // 电池电压采集
extern unsigned char Camera_Flag; // 场标识
extern uint8_t CircleNumber;// 入环次数，0结束；默认1次 ;环的个数一般在比赛前测试赛道时就知道了
extern int16_t OFFSET0;    // 最远处，赛道中心值综合偏移量
extern int16_t OFFSET1;    // 第二格
extern int16_t OFFSET2;    // 最近，第三格

void EncInit(void);
void MotorInit(u16 freq);
void MotorCtrl(int16_t motor1, int16_t motor2);
void MotorCtrl4w(int16_t motor1, int16_t motor2,int16_t motor3, int16_t motor4);
void BalMotorCtrl(int16_t motor1, int16_t motor2);
void TestMotor(void);
void ServoInit(u16 freq);
void ServoCtrl(uint16_t duty);
void TestServo(void);
void SpeedCtrl(signed short x, signed short y, signed short z);
#endif /* SRC_APPSW_TRICORE_USER_LQ_MOTORCONTROL_H_ */
