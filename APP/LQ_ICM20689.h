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
#ifndef __LQ_ICM20689_H
#define __LQ_ICM20689_H

#define ICM20689_ADDR                   0x68  //IIC写入时的地址字节数据，+1为读取
#define ICM20689_ADDR2                  0x69  //取决于AD0的状态

#define ICM20689_ID	                0x98	//IIC地址寄存器(默认数值0x98，只读)

#define ICM_SELF_TEST_X_GYRO            0x00
#define ICM_SELF_TEST_Y_GYRO            0x01 
#define ICM_SELF_TEST_Z_GYRO            0x02

#define ICM_SELF_TESTX_REG		0X0D	//ACCEL自检寄存器X
#define ICM_SELF_TESTY_REG		0X0E	//ACCEL自检寄存器Y
#define ICM_SELF_TESTZ_REG		0X0F	//ACCEL自检寄存器Z

#define ICM_SELF_TESTA_REG		0X10	//自检寄存器A

#define ICM_XG_OFFS_USRH                0x13	
#define ICM_XG_OFFS_USRL                0x14
#define ICM_YG_OFFS_USRH                0x15
#define ICM_YG_OFFS_USRL                0x16
#define ICM_ZG_OFFS_USRH                0x17
#define ICM_ZG_OFFS_USRL                0x18

#define ICM_SAMPLE_RATE_REG		0X19	//采样频率分频器
#define ICM_CFG_REG			0X1A	//配置寄存器
#define ICM_GYRO_CFG_REG		0X1B	//陀螺仪配置寄存器
#define ICM_ACCEL_CFG_REG		0X1C	//加速度计配置寄存器
#define ICM_ACCEL_CONFIG_2              0x1D
#define ICM_LOW_POWER_MODE		0x1E
#define ICM_MOTION_DET_REG		0X1F	//运动检测阀值设置寄存器
#define ICM_FIFO_EN_REG			0X23	//FIFO使能寄存器

#define ICM_I2CMST_STA_REG		0X36	//IIC主机状态寄存器
#define ICM_INTBP_CFG_REG		0X37	//中断/旁路设置寄存器
#define ICM_INT_EN_REG			0X38	//中断使能寄存器
#define ICM_INT_STA_REG			0X3A	//中断状态寄存器

#define ICM_ACCEL_XOUTH_REG		0X3B	//加速度值,X轴高8位寄存器
#define ICM_ACCEL_XOUTL_REG		0X3C	//加速度值,X轴低8位寄存器
#define ICM_ACCEL_YOUTH_REG		0X3D	//加速度值,Y轴高8位寄存器
#define ICM_ACCEL_YOUTL_REG		0X3E	//加速度值,Y轴低8位寄存器
#define ICM_ACCEL_ZOUTH_REG		0X3F	//加速度值,Z轴高8位寄存器
#define ICM_ACCEL_ZOUTL_REG		0X40	//加速度值,Z轴低8位寄存器

#define ICM_TEMP_OUTH_REG		0X41	//温度值高八位寄存器
#define ICM_TEMP_OUTL_REG		0X42	//温度值低8位寄存器

#define ICM_GYRO_XOUTH_REG		0X43	//陀螺仪值,X轴高8位寄存器
#define ICM_GYRO_XOUTL_REG		0X44	//陀螺仪值,X轴低8位寄存器
#define ICM_GYRO_YOUTH_REG		0X45	//陀螺仪值,Y轴高8位寄存器
#define ICM_GYRO_YOUTL_REG		0X46	//陀螺仪值,Y轴低8位寄存器
#define ICM_GYRO_ZOUTH_REG		0X47	//陀螺仪值,Z轴高8位寄存器
#define ICM_GYRO_ZOUTL_REG		0X48	//陀螺仪值,Z轴低8位寄存器

#define ICM_I2CSLV0_DO_REG		0X63	//IIC从机0数据寄存器
#define ICM_I2CSLV1_DO_REG		0X64	//IIC从机1数据寄存器
#define ICM_I2CSLV2_DO_REG		0X65	//IIC从机2数据寄存器
#define ICM_I2CSLV3_DO_REG		0X66	//IIC从机3数据寄存器

#define ICM_I2CMST_DELAY_REG	        0X67	//IIC主机延时管理寄存器
#define ICM_SIGPATH_RST_REG		0X68	//信号通道复位寄存器
#define ICM_MDETECT_CTRL_REG	        0X69	//运动检测控制寄存器
#define ICM_USER_CTRL_REG		0X6A	//用户控制寄存器
#define ICM_PWR_MGMT1_REG		0X6B	//电源管理寄存器1
#define ICM_PWR_MGMT2_REG		0X6C	//电源管理寄存器2 
#define ICM_FIFO_CNTH_REG		0X72	//FIFO计数寄存器高八位
#define ICM_FIFO_CNTL_REG		0X73	//FIFO计数寄存器低八位
#define ICM_FIFO_RW_REG			0X74	//FIFO读写寄存器

#define WHO_AM_I		        0X75	//器件ID寄存器

void ICM_SPI_Init(void);
unsigned char ICM_Read_Byte(unsigned char reg);
void  ICM_Write_Byte(unsigned char reg,unsigned char value);
void  ICM_Read_Len(unsigned char reg,unsigned char len,unsigned char *buf);
void  ICM_Get_Raw_data(unsigned short *ax,unsigned short *ay,unsigned short *az,unsigned short *gx,unsigned short *gy,unsigned short *gz);
void  ICM_Get_Accelerometer(unsigned short *ax,unsigned short *ay,unsigned short *az);
void  ICM_Get_Gyroscope(unsigned short *gx,unsigned short *gy,unsigned short *gz);
unsigned short ICM_Get_Temperature(void);
void  ICM_Set_Rate(unsigned short rate);
void  ICM_Set_LPF(unsigned short lpf);
void  ICM_Set_Accel_Fsr(unsigned char fsr);
void  ICM_Set_Gyro_Fsr(unsigned char fsr);
unsigned char ICM20689_Init(void);
void Test_ICM20689(void);
void Test_ICM20689DMP(void);
void icm20689_reset(void);

#endif


