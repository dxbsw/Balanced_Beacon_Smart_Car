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

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】chiusir
【功能说明】读取 ICM20689 的原始数据
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

void Test_ICM20689(void)
{
  char  txt[16];
  unsigned char cnt=0; 
  u16 aacx,aacy,aacz;	 //加速度传感器原始数据
  u16 gyrox,gyroy,gyroz; //陀螺仪原始数据   
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(1, 0,"LQ20689M Test", u16PURPLE, u16BLACK);   // 字符串显示
  if(ICM20689_Init())
  {       
    TFTSPI_P8X16Str(1, 2,"LQ20689M Fail", u16PURPLE, u16BLACK);   // 字符串显示
    //while(1)     
  }
  
  while(1)
  {
    ICM_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);	//得到加速度传感器数据  
    sprintf((char*)txt,"ax:%06d",aacx);
    TFTSPI_P8X16Str(1, 3, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf((char*)txt,"ay:%06d",aacy);
    TFTSPI_P8X16Str(1, 4, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf((char*)txt,"az:%06d",aacz);
    TFTSPI_P8X16Str(1, 5, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf((char*)txt,"gx:%06d",gyrox);
    TFTSPI_P8X16Str(1, 6, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf((char*)txt,"gy:%06d",gyroy);
    TFTSPI_P8X16Str(1, 7, txt, u16PURPLE, u16BLACK);   // 字符串显示
    sprintf((char*)txt,"gz:%06d",gyroz);
    TFTSPI_P8X16Str(1, 8, txt, u16PURPLE, u16BLACK);   // 字符串显示
    
    cnt++; 
    sprintf((char*)txt,"%03d",cnt);
    TFTSPI_P8X16Str(10, 1, txt, u16PURPLE, u16BLACK);  
    LED_Ctrl(LED1,RVS); //LED翻转闪烁
    
    //ANO_DT_send_int16(0,0,aacx,aacy,aacz,gyrox,gyroy,gyroz);
    
  }  
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【作  者】
【功能说明】初始化 ICM20689
【软件版本】V1.0
【最后更新】2019年03月13日 
【函数名】
【返回值】无 0：初始化成功   1：失败
【参数值】无 
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u8 ICM20689_Init(void)
{
  char  txt[10];
  u8 res;
  
  ICM_SPI_Init(); 
  delayms(100);  
  res=ICM_Read_Byte(WHO_AM_I);                         //读取ICM20689的ID  
  sprintf(txt,"ID:0x%02X",res);  
  TFTSPI_P8X16Str(1, 1, txt, u16PURPLE, u16BLACK);     // 字符串显示
  if(res!=ICM20689_ID)                                 //器件ID正确
  {    
    //printf("ID=%#X\r\n",res);
    //OLED_P6x8Str(1,4,"ID is fail!");
    return 1;
  }
  else  
  {
    //printf("ICM20689 is OK!\n");  
  }   
  
  ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X80);//复位
  delayms(100);  //延时100ms
  ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X00);//唤醒
  delayms(100);  //延时100ms
  
  ICM_Set_Gyro_Fsr(3);			 //陀螺仪传感器,±2000dps   
  ICM_Set_Accel_Fsr(1);			 //加速度传感器,0:±2g   1:±4g    2:±8g
  ICM_Set_Rate(1000);			 //设置采样率1000Hz
  //ICM_Write_Byte(ICM_CFG_REG,0x02);    //设置数字低通滤波器   98hz
  ICM_Write_Byte(ICM_CFG_REG,4);	 //低通滤波频率，典型值：0x07(3600Hz)此寄存器内决定Internal_Sample_Rate==8K
  ICM_Write_Byte(ICM_INT_EN_REG,0X00);   //关闭所有中断
  ICM_Write_Byte(ICM_FIFO_EN_REG,0X00);	 //关闭FIFO
  ICM_Write_Byte(ICM_USER_CTRL_REG,0X00);//I2C主模式关闭
  ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
  ICM_Write_Byte(ICM_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作 
  
  return 0;
}

/*************************************************************************
*  函数名称：void SPI_SoftInit(void)
*  功能说明：模拟SPI口初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/

void ICM_SPI_Init(void)
{
#ifdef USE_SOFT_SPI
  Soft_SPI_Init();
#else
  SPIM_Init(SPI1, SPI_BaudRatePrescaler_32);//0x20
#endif
}


/*************************************************************************
*  函数名称：void ICM_Read_Len(u8 reg,u8 len,u8 *buf)
*  功能说明：u8 reg起始寄存器,u8 *buf数据指针,u16 len长度
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Read_Len(u8 reg,u8 len,u8 *buf)
{  
#ifdef USE_SOFT_SPI
  buf[0] = reg | 0x80;  
  Soft_SPI_ReadWriteNbyte(buf, len + 1);  
#else
  SPI_Read_Nbytes(SPI1,reg|0x80,buf,len);
#endif
}
/*************************************************************************
*  函数名称：void ICM_Get_Gyroscope(u16 *gx,u16 *gy,u16 *gz)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Get_Gyroscope(u16 *gx,u16 *gy,u16 *gz)
{
  u8 buf[7]; 
  ICM_Read_Len(ICM_GYRO_XOUTH_REG,6,buf);
  
  *gx=((u16)buf[1]<<8)|buf[2];  
  *gy=((u16)buf[3]<<8)|buf[4];  
  *gz=((u16)buf[5]<<8)|buf[6];
  
}
/*************************************************************************
*  函数名称：void ICM_Get_Accelerometer(u16 *ax,u16 *ay,u16 *az)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Get_Accelerometer(u16 *ax,u16 *ay,u16 *az)
{
  u8 buf[7];  
  ICM_Read_Len(ICM_ACCEL_XOUTH_REG,6,buf);
  
  *ax=((u16)buf[1]<<8)|buf[2];  
  *ay=((u16)buf[3]<<8)|buf[4];  
  *az=((u16)buf[5]<<8)|buf[6];  
}

/*************************************************************************
*  函数名称：void ICM_Get_Raw_data(u16 *ax,u16 *ay,u16 *az,u16 *gx,u16 *gy,u16 *gz)
*  功能说明：读取加速度陀螺仪数据
*  参数说明：六轴
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Get_Raw_data(u16 *ax,u16 *ay,u16 *az,u16 *gx,u16 *gy,u16 *gz)
{
  u8 buf[16];  
  ICM_Read_Len(ICM_ACCEL_XOUTH_REG,14,buf);
  
  *ax=((u16)buf[1]<<8)|buf[2];  
  *ay=((u16)buf[3]<<8)|buf[4];  
  *az=((u16)buf[5]<<8)|buf[6];
  *gx=((u16)buf[9]<<8)|buf[10];  
  *gy=((u16)buf[11]<<8)|buf[12];  
  *gz=((u16)buf[13]<<8)|buf[14];
}

/*************************************************************************
*  函数名称：void ICM_Write_Byte(u8 reg,u8 value)
*  功能说明：向寄存器写数据
*  参数说明：u8 reg寄存器,u8 value数据
*  函数返回：无
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Write_Byte(u8 reg,u8 value)
{
#ifdef USE_SOFT_SPI
  uint8_t buff[2];  
  buff[0] = reg;          //先发送寄存器
  buff[1] = value;        //再发送数据  
  Soft_SPI_ReadWriteNbyte(buff, 2);//发送buff里数据，并采集到 buff里  
  
#else
  SPIM_CSLow(SPI1);
  SPI_WriteRead(SPI1, reg | 0x80);
  SPI_WriteRead(SPI1, value);
  SPIM_CSHigh(SPI1);  
#endif
}

/*************************************************************************
*  函数名称：void ICM_Write_Byte(u8 reg,u8 value)
*  功能说明：向寄存器写数据
*  参数说明：u8 reg寄存器,
*  函数返回：u8 value数据
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
u8 ICM_Read_Byte(u8 reg)
{  
  u8 buff[2];
#ifdef USE_SOFT_SPI
  buff[0] = reg | 0x80;          //先发送寄存器  
  Soft_SPI_ReadWriteNbyte(buff, 2);   
  return buff[1];
#else
  SPIM_CSLow(SPI1);
  SPI_WriteRead(SPI1, reg | 0x80);
  buff[1]=SPI_WriteRead(SPI1, 0);
  SPIM_CSHigh(SPI1);
  return buff[1];
  
#endif  
}
/*************************************************************************
*  函数名称：void ICM_Set_Gyro_Fsr(u8 fsr)
*  功能说明：设置陀螺仪传感器满量程范围
*  参数说明：Fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Set_Gyro_Fsr(u8 fsr)
{
  ICM_Write_Byte(ICM_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
/*************************************************************************
*  函数名称：void ICM_Set_Accel_Fsr(u8 fsr)
*  功能说明：设置LQ20689陀螺仪传感器满量程范围
*  参数说明：fsr:0,±2g;1,±4g;2,±8g;3,±16g
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Set_Accel_Fsr(u8 fsr)
{
  ICM_Write_Byte(ICM_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
/*************************************************************************
*  函数名称：void ICM_Set_LPF(u16 lpf)
*  功能说明：设置LQ20689数字低通滤波器
*  参数说明：lpf:数字低通滤波频率(Hz)
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Set_LPF(u16 lpf)
{
  u8 dat=0;
  if(lpf>=188)dat=1;
  else if(lpf>=98)dat=2;
  else if(lpf>=42)dat=3;
  else if(lpf>=20)dat=4;
  else if(lpf>=10)dat=5;
  else dat=6; 
  ICM_Write_Byte(ICM_CFG_REG,dat);//设置数字低通滤波器  
}
/*************************************************************************
*  函数名称：void ICM_Set_Rate(u16 rate)
*  功能说明：设置LQ20689陀螺仪传感器满量程范围
*  参数说明：rate:4~1000(Hz)
*  函数返回：
*  修改时间：2020年8月10日
*  备    注：   
*************************************************************************/
void ICM_Set_Rate(u16 rate)
{
  u8 dat;
  if(rate>1000)rate=1000;
  if(rate<4)rate=4;
  dat=1000/rate-1;
  ICM_Write_Byte(ICM_SAMPLE_RATE_REG,dat);	//设置数字低通滤波器
  ICM_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}
/*************************************************************************
*  函数名称：u16 ICM_Get_Temperature(void)
*  功能说明：得到温度值
*  参数说明：
*  函数返回：返回值:温度值(扩大了100倍)
*  修改时间：2020年8月10日
*  备    注：返回值:温度值(扩大了100倍)   
*************************************************************************/
u16 ICM_Get_Temperature(void)
{
  u8 buf[3]; 
  u16 raw;
  float temp;
  ICM_Read_Len(ICM_TEMP_OUTH_REG,2,buf); 
  raw=((u16)buf[0]<<8)|buf[1];  
  temp=21+((double)raw)/333.87;  
  return (u16)temp*100;
}


//**********************************************************************************************************************************************************
void icm20689_reset(void)
{
  ICM_Write_Byte(ICM_PWR_MGMT1_REG,0X80);	//复位
  delayms(100);
  ICM_Write_Byte(ICM_PWR_MGMT1_REG, 0x00);	//解除休眠状态
}


//


