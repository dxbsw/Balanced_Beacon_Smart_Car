/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef __LQ_6050_H_
#define __LQ_6050_H_

#include "include.h"

//AD0引脚接了GND，地址为0x68
#define MPU6050_ADDR    0x68  //MPU6500的器件地址


//MPU6500的内部寄存器
#define MPU_SELF_TESTX_GYRO_REG     0X00    //陀螺仪X轴自检寄存器
#define MPU_SELF_TESTY_GYRO_REG     0X01    //陀螺仪Y轴自检寄存器
#define MPU_SELF_TESTZ_GYRO_REG     0X02    //陀螺仪Z轴自检寄存器

#define MPU_SELF_TESTX_ACCEL_REG    0X0D    //加速度计自检寄存器X
#define MPU_SELF_TESTY_ACCEL_REG    0X0E	 //加速度计自检寄存器Y
#define MPU_SELF_TESTZ_ACCEL_REG    0X0F	 //加速度计自检寄存器Z

#define MPU_SAMPLE_RATE_REG         0X19	 //采样频率分频器
#define MPU_CFG_REG		         0X1A	 //配置寄存器
#define MPU_GYRO_CFG_REG		     0X1B	 //陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG		     0X1C	 //加速度计配置寄存器
#define MPU_MOTION_DET_REG		    0X1F	//运动检测阀值设置寄存器
#define MPU_FIFO_EN_REG	         0X23	 //FIFO使能寄存器

#define MPU_I2CMST_STA_REG		     0X36	//IIC主机状态寄存器
#define MPU_INTBP_CFG_REG           0X37	//中断/旁路设置寄存器
#define MPU_INT_EN_REG			     0X38	//中断使能寄存器
#define MPU_INT_STA_REG	         0X3A	//中断状态寄存器

#define MPU_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define MPU_TEMP_OUTH_REG		    0X41	//温度值高八位寄存器
#define MPU_TEMP_OUTL_REG		    0X42	//温度值低8位寄存器

#define MPU_GYRO_XOUTH_REG		    0X43	//陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG		    0X44	//陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG		    0X45	//陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG		    0X46	//陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG		    0X47	//陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG		    0X48	//陀螺仪值,Z轴低8位寄存器

#define MPU_I2CSLV0_DO_REG		    0X63	//IIC从机0数据寄存器
#define MPU_I2CSLV1_DO_REG		    0X64	//IIC从机1数据寄存器
#define MPU_I2CSLV2_DO_REG		    0X65	//IIC从机2数据寄存器
#define MPU_I2CSLV3_DO_REG		    0X66	//IIC从机3数据寄存器

#define MPU_I2CMST_DELAY_REG	    0X67	//IIC主机延时管理寄存器
#define MPU_SIGPATH_RST_REG        0X68	//信号通道复位寄存器
#define MPU_MDETECT_CTRL_REG	    0X69	//运动检测控制寄存器
#define MPU_USER_CTRL_REG		    0X6A	//用户控制寄存器
#define MPU_PWR_MGMT1_REG		    0X6B	//电源管理寄存器1
#define MPU_PWR_MGMT2_REG		    0X6C	//电源管理寄存器2
#define MPU_FIFO_CNTH_REG		    0X72	//FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG		    0X73	//FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG	        0X74	//FIFO读写寄存器
#define WHO_AM_I		            0X75	//器件ID寄存器

 void Test_MPU6050(void);
unsigned char MPU6050_Init(void);
unsigned char MPU6050_WaitForReady(unsigned char addr);
unsigned char MPU6050_Write_Byte(unsigned char addr,unsigned char reg,unsigned char data);
unsigned char MPU6050_Read_Byte(unsigned char addr,unsigned char reg);
unsigned char MPU6050_Set_Gyro_Fsr(unsigned char fsr);
unsigned char MPU6050_Set_Accel_Fsr(unsigned char fsr);
unsigned char MPU6050_Set_Rate(unsigned short rate);
unsigned char MPU6050_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);
unsigned char MPU6050_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf);
short MPU6050_Get_Temperature(void);
unsigned char MPU6050_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz);
unsigned char MPU6050_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az);
unsigned char MPU6050_Get_Magnetometer(short *mx,short *my,short *mz);
unsigned char MPU6050_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz);
float MPU6050_Get_Angle(short accx, short accy, short accz, char choose);


#endif

