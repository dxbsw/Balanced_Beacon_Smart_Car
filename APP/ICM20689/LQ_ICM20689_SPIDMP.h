/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技F3277核心板和母板
【编    写】龙邱科技
【E-mail  】chiusir@163.com
【软件版本】V1.0 版权所有，单位使用请先联系授权
【最后更新】2021年3月31日，持续更新，请关注最新版！
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【IDE】IAR7.8 KEIL5.24及以上版本
【Target 】 MM32F3277
【SYS PLL】 120MHz 频率太高可能无法启动system_mm32f327x.c
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
=================================================================
本程序测试_LQ_ICM20689_DMP_功能，配置匿名地面站4.34显示波形
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_ICM20689_DMP_
#define _LQ_ICM20689_DMP_

#include "LQ_SOFTI2C.h"

extern short   gyro[3], accel[3], sensors;
extern float   Pitch, Roll; 

extern int LQ_I2C_Write(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *dat);
extern int LQ_I2C_Read(unsigned char addr, unsigned char reg, unsigned int len, unsigned char *buf);
extern void delayms(u16 nms);
extern void delayus(u16 nus);

void Test_LQDMP(void);
int LQ_DMP_Init(void);
int LQ_DMP_Read(void);
int LQ_ICM20689_DMP_Init(void);
int LQ_ICM20689_DMP_Read(void);

// 初始化相关
int icm20689_disable_dmp(unsigned char wake_state);
int icm20689_disable_fifo(unsigned char wake_state);
int icm20689_reset_fifo(unsigned char wake_state);
int icm20689_enable_fifo(unsigned char wake_state);
int icm20689_enable_dmp(unsigned char wake_state);
int icm20689_write_mems(unsigned char wake_state, unsigned int reg, unsigned int len, unsigned char *buf);

// 读值相关
long inv_icm20689_convert_mult_q30_fxp(long a_q30, long b_q30);
long inv_icm20689_convert_fast_sqrt_fxp(long x0_q30);
int inv_icm20689_convert_test_limits_and_scale_fxp(long *x0_q30, int *pow);
int inv_icm20689_convert_get_highest_bit_position(unsigned long *value);
	
// 无用
int do_test(int self_test_flag, int *gyro_result, int *accel_result, int lp_mode);
	
#endif
 