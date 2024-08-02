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
#ifndef SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_
#define SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_

extern volatile u8 Game_Over; // 小车完成全部任务，停车
extern int16_t Offset ;
extern u16 MagneticField;     // 磁场强度
extern int16_t LleftFP, LleftFV, LrightFV, LrightFP;  // 电感偏移量
extern int16_t LleftBP, LleftBV, LrightBV, LrightBP;  // 电感偏移量

/*************************************************************************
*  函数名称：void InductorInit (void)
*  功能说明：四个电感ADC初始化函数；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void InductorInit(void);

/*************************************************************************
*  函数名称：void InductorNormal(void)
*  功能说明：采集电感电压并并归一化；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void InductorNormal(void);

/*************************************************************************
*  函数名称：uint8 ReadOutInGarageMode(void)
*  功能说明：读取拨码开关设置出入库模式
*  参数说明：无
*  函数返回：出入库模式,0左出入库；默认1右出入库
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
uint8_t ReadOutInGarageMode(void);

/*************************************************************************
*  函数名称：void ElectroMagneticCar(void)
*  功能说明：电磁车双电机差速控制
-->1.入门算法：简单的分段比例控制算法，教学演示控制算法；
2.进阶算法：PID典型应用控制算法，教学演示控制算法；
3.高端算法：改进粒子群协同控制算法；
*  参数说明：无
*  函数返回：无
*  修改时间：2020年10月28日
*  备    注：驱动2个电机
*************************************************************************/
void  ElectroMagneticMWCar (void);

/*************************************************************************
*  函数名称：void TFT_Show_EleMag_Info(void)
*  功能说明：显示各种所需信息
*  参数说明：无
*  函数返回：无
*  修改时间：2020年11月18日
*  备    注：
*************************************************************************/
void TFT_Show_EleMag_Info(void);
#endif /* SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_ */
