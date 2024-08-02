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
#include <LQ_PID.h>

/*************************************************************************
*  函数名称：float constrain_float(float amt, float low, float high)
*  功能说明：限幅函数
*  参数说明：
* @param    amt   ： 参数
* @param    low   ： 最低值
* @param    high  ： 最高值
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float constrain_float(float amt, float low, float high)
{
  return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// pid参数初始化函数
void PidInit(pid_param_t * pid)
{
  pid->kp        = 0;
  pid->ki        = 0;
  pid->kd        = 0;
  pid->imax      = 0;
  pid->out_p     = 0;
  pid->out_i     = 0;
  pid->out_d     = 0;
  pid->out       = 0;
  pid->integrator= 0;
  pid->last_error= 0;
  pid->last_derivative   = 0;
  pid->last_t    = 0;
}

/*************************************************************************
*  函数名称：float constrain_float(float amt, float low, float high)
*  功能说明：pid位置式控制器输出
*  参数说明：
* @param    pid     pid参数
* @param    error   pid输入误差
*  函数返回：PID输出结果
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
  /* 累积误差 */
  pid->integrator += error;
  
  /* 误差限幅 */
  constrain_float(pid->integrator, -pid->imax, pid->imax);
  
  
  pid->out_p = pid->kp * error;
  pid->out_i = pid->ki * pid->integrator;
  pid->out_d = pid->kd * (error - pid->last_error);
  
  pid->last_error = error;
  
  pid->out = pid->out_p + pid->out_i + pid->out_d;
  
  return pid->out;
}
/*************************************************************************
*  函数名称：float constrain_float(float amt, float low, float high)
*  功能说明：pid增量式控制器输出
*  参数说明：
* @param    pid     pid参数
* @param    error   pid输入误差
*  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
*  修改时间：2020年4月1日
*  备    注：
*************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
  
  pid->out_p = pid->kp * (error - pid->last_error);
  pid->out_i = pid->ki * error;
  pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);
  
  pid->last_derivative = error - pid->last_error;
  pid->last_error = error;
  
  pid->out += pid->out_p + pid->out_i + pid->out_d;
  
  return pid->out;
}

/*************************************************************************

变积分

************************************************************************/
struct B_pid {
     float SetSpeed; //定义设定值
     float ActualSpeed; //定义实际值
     float err; //定义偏差值
     float err_last; //定义上一个偏差值
     float Kp, Ki, Kd; //定义比例、积分、微分系数
     float SetDuty; //定义（控制执行器的变量）
     float integral; //定义积分值
 }Bj_pid;



void PID_init(void) {
     Bj_pid.SetSpeed = 0.0;
     Bj_pid.ActualSpeed = 0.0;
     Bj_pid.err = 0.0;
     Bj_pid.err_last = 0.0;
     Bj_pid.SetDuty = 0.0;
     Bj_pid.integral = 0.0;
     Bj_pid.Kp = 0.8;
     Bj_pid.Ki = 0;    //增加了积分系数
     Bj_pid.Kd = 0.40;

 }
/******************************************************
set_speed   设定值
actual_speed 输入值

*******************************************************/
 float PID_realize(float set_speed,float actual_speed) {
     float index;
     Bj_pid.SetSpeed = 100;
     Bj_pid.ActualSpeed = (actual_speed/set_speed)*100;
     Bj_pid.err = Bj_pid.SetSpeed - Bj_pid.ActualSpeed;

     if (abs(Bj_pid.err) > 30)           //变积分过程
     {
         index = 0;
     }
     else if (abs(Bj_pid.err) <20) {
         index = 1.0;
         Bj_pid.integral += Bj_pid.err;
     }
     else {
         index = (100 - abs(Bj_pid.err)) / 10;
         Bj_pid.integral += Bj_pid.err;
     }
     Bj_pid.SetDuty = Bj_pid.Kp*Bj_pid.err + index * Bj_pid.Ki*Bj_pid.integral + Bj_pid.Kd*(Bj_pid.err - Bj_pid.err_last);

     Bj_pid.err_last = Bj_pid.err;
     if(Bj_pid.SetDuty>100)Bj_pid.SetDuty=100;
     if(Bj_pid.SetDuty<0)Bj_pid.SetDuty=0;
     return Bj_pid.SetDuty;
 }