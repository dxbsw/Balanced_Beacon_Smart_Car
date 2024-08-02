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
#ifndef SRC_APPSW_TRICORE_USER_LQ_PID_H_
#define SRC_APPSW_TRICORE_USER_LQ_PID_H_

#include <stdint.h>

typedef struct
{
  float                kp;         //P
  float                ki;         //I
  float                kd;         //D
  float                imax;       //积分限幅
  
  float                out_p;  //KP输出
  float                out_i;  //KI输出
  float                out_d;  //KD输出
  float                out;    //pid输出
  
  float                integrator; //< 积分值
  float                last_error; //< 上次误差
  float                last_derivative;//< 上次误差与上上次误差之差
  unsigned long        last_t;     //< 上次时间
}pid_param_t;




void PidInit(pid_param_t * pid);

float constrain_float(float amt, float low, float high);

float PidLocCtrl(pid_param_t * pid, float error);

float PidIncCtrl(pid_param_t * pid, float error);




void PID_init(void);
float PID_realize(float set_speed,float actual_speed);

#endif /* SRC_APPSW_TRICORE_USER_LQ_PID_H_ */
