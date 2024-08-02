/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技SPIN27PS核心板
 【编    写】chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年10月28日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【IDE】MM32SPIN27PS:IAR7.8/MDK5.2及以上版本  
 【Target 】 SPIN27PS
 【SYS PLL】 80MHz
=================================================================
程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_I2C_MPU6050.h>
#include <LQ_TFT18.h>
#include <LQ_SOFTI2C.h>
#include <stdio.h>

#include <LQ_UART.h>

#include <math.h>
/**
  * @brief    不精确延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      delayms_mpu9250(100);
  *
  * @date     2020/12/10 星期四
*/
void delayms_mpu6050(uint16_t ms)
{
    int i;
  for (i = 0; i < 300; ++i)
  {
    __asm("NOP"); /* delay */
  }
//  while(ms--)
//  {
//      uint16_t  i = 300;
//      while(i--)
//      {
//          NOP(50);
//      }
//  }
}


/**
  * @brief    初始化MPU6050
  *
  * @param    无
  *
  * @return   0：初始化成功   1：失败
  *
  * @note     使用前先初始化IIC接口
  *
  * @see      MPU9250_Init();
  *
  * @date     2020/12/15 星期二
  */
unsigned char MPU6050_Init(void)
{
    int  res;
    SOFT_IIC_Init();                                           //MPU9250 支持400K IIC

    res = 0;
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//复位MPU9250
    delayms_mpu6050(100);  //延时100ms
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//唤醒MPU9250
    res += MPU6050_Set_Gyro_Fsr(3);                                //陀螺仪传感器,±2000dps
    res += MPU6050_Set_Accel_Fsr(1);                               //加速度传感器,±4g
    res += MPU6050_Set_Rate(1000);                                 //设置采样率1000Hz
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //关闭所有中断
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //设置数字低通滤波器   98hz
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C主模式关闭
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);  //关闭FIFO
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X82);//INT引脚低电平有效，开启bypass模式
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);//设置CLKSEL,PLL X轴为参考
    res += MPU6050_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);//加速度与陀螺仪都工作


    if(res == 0)  //上面寄存器都写入成功
    {
 //       UART_PutStr(UART1, "MPU6050 set is OK!\r\n");
    }
    else{
//        UART_PutStr(UART1, "MPU6050 set is error!\r\n");
        return 1;
    }

    res = MPU6050_Read_Byte(MPU6050_ADDR,WHO_AM_I);     //读取MPU6500的ID
/*    if( (res == MPU6500_ID1) )  //器件ID正确
        UART_PutStr(UART1, "MPU6500_ID1 is OK!\r\n");
    else
    {
        //printf("ID=%#X\r\n",res);
        //printf("MPU9250 is fail!\n");
        UART_PutStr(UART1,"MPU6500_ID1 is error!\r\n");
        UART_PutChar(UART1, res);
        UART_PutStr(UART1,"\r\n");
        UART_PutChar(UART1, MPU6500_ID1);
        UART_PutStr(UART1,"\r\n");
        while(1);
    }*/


    return 0;
}




/**
  * @brief    设置陀螺仪测量范围
  *
  * @param    fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
  *
  * @return   0 ：设置成功
  *
  * @note     无
  *
  * @see      MPU9250_Set_Gyro_Fsr(3);        //陀螺仪传感器,±2000dps
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Set_Gyro_Fsr(uint8_t fsr)
{
    return MPU6050_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}



/**
  * @brief    设置加速度计测量范围
  *
  * @param    fsr:0,±2g;1,±4g;2,±8g;3,±16g
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU9250_Set_Accel_Fsr(1);       //加速度传感器,±4g
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Set_Accel_Fsr(uint8_t fsr)
{
    return MPU6050_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}



/**
  * @brief    设置数字低通滤波
  *
  * @param    lpf:数字低通滤波频率(Hz)
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU6050_Set_LPF(100);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Set_LPF(uint16_t lpf)
{
    unsigned char  data=0;
    if(lpf>=188)data=1;
    else if(lpf>=98)data=2;
    else if(lpf>=42)data=3;
    else if(lpf>=20)data=4;
    else if(lpf>=10)data=5;
    else data=6;
    return MPU6050_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,data);//设置数字低通滤波器
}




/**
  * @brief    设置采样率
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0：设置成功
  *
  * @note     无
  *
  * @see      MPU9250_Set_Rate(1000);              //设置采样率1000Hz
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Set_Rate(uint16_t rate)
{
    unsigned char  data;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    data = 1000/rate-1;
    MPU6050_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,data);      //设置数字低通滤波器
    return MPU6050_Set_LPF(rate/2);                                 //自动设置LPF为采样率的一半
}



/**
  * @brief    获取温度值
  *
  * @param    无
  *
  * @return   温度值(扩大了100倍)
  *
  * @note     无
  *
  * @see      signed short temp = MPU9250_Get_Temperature();
  *
  * @date     2020/12/10 星期四
  */
short MPU6050_Get_Temperature(void)
{
    unsigned char  buf[2];
    short raw;
    float temp;
    MPU6050_Read_Len(MPU6050_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16_t)buf[0]<<8)|buf[1];
    temp=21+((double)raw)/333.87;
    return (short)temp*100;
}





/**
  * @brief    获取陀螺仪值
  *
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[3];
  * @see      MPU9250_Get_Gyroscope(&data[0], &data[1], &data[2]);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[6],res;
    res=MPU6050_Read_Len(MPU6050_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16_t)buf[0]<<8)|buf[1];
        *gy=((uint16_t)buf[2]<<8)|buf[3];
        *gz=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;
}

/**
  * @brief    获取加速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[3];
  * @see      MPU9250_Get_Accelerometer(&data[0], &data[1], &data[2]);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
    unsigned char  buf[6],res;
    res=MPU6050_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
    }
    return res;
}

/**
  * @brief    获取 加速度值 角速度值
  *
  * @param    ax,ay,az:陀螺仪x,y,z轴的原始读数(带符号)
  * @param    gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0：读取成功
  *
  * @note     无
  *
  * @see      signed short data[6];
  * @see      MPU9250_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[14],res;
    res=MPU6050_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
    if(res==0)
    {
        *ax=((uint16_t)buf[0]<<8)|buf[1];
        *ay=((uint16_t)buf[2]<<8)|buf[3];
        *az=((uint16_t)buf[4]<<8)|buf[5];
        *gx=((uint16_t)buf[8]<<8)|buf[9];
        *gy=((uint16_t)buf[10]<<8)|buf[11];
        *gz=((uint16_t)buf[12]<<8)|buf[13];
    }
    return res;
}

/**
  * @brief    IIC 连续写
  *
  * @param    addr:器件地址
  * @param    reg :要写入的寄存器地址
  * @param    len :要写入的长度
  * @param    buf :写入到的数据存储区
  *
  * @return   0 ：写入成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}


/**
  * @brief    IIC 连续读
  *
  * @param    addr:器件地址
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}



/**
  * @brief    IIC 写一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU9250_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,1);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}


/**
  * @brief    IIC 读一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :寄存器地址
  *
  * @return   读取的值
  *
  * @note     底层驱动 移植时需要修改
  *
  * @see      MPU9250_Read_Byte(MPU9250_ADDR,WHO_AM_I);
  *
  * @date     2020/12/10 星期四
  */
unsigned char MPU6050_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    IIC_ReadByteFromSlave(addr<<1, reg, &value[0]);
    return value[0];
}

// 通过加速度计获取绝对坐标
float MPU6050_Get_Angle(short accx, short accy, short accz, char choose)
{
    double temp;
    float res = 0;
    switch(choose)
    {
        case 0 ://与自然x轴的角度
            temp = (float)accx / sqrt( (accy * accy + accz * accz) );
            res = atan(temp);
            break;
        case 1 ://与自然y轴的角度
            temp = (float)accy / sqrt((accx * accx + accz * accz));
            res = atan(temp);
            break;
        case 2 ://与自然z轴的角度
            temp = (float)accz / sqrt(accx * accx + accy * accy);
            res = atan(temp);
            break;
    }
    return res * 1800 / 3.1415;
}


//测试程序
//单位为0.1毫米每秒
void Test_MPU6050(void)
{
    TFTSPI_Init(1);     //TFT1.8初始化
    TFTSPI_CLS(u16BLACK);    //清屏
    char txt[30];
    short aacx, aacy, aacz;
    short gyrox, gyroy, gyroz;
    MPU6050_Set_LPF(10);
    SOFT_IIC_Init();
    TFTSPI_P8X16Str(2,0,"LQ 6050 Test",u16RED,u16BLACK);
//    UART_PutStr(UART1, "LQ MPU6050 Test\r\n");
    MPU6050_Init();
    if(MPU6050_Init())
    {
        TFTSPI_P8X16Str(1,0,"6050 Test Fail",u16RED,u16BLACK);
//        UART_PutStr(UART1,"MPU6050 init Fail\r\n");
        while(1);
    }
    while(1)
    {
        MPU6050_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //得到加速度传感器数据
        sprintf((char*)txt,"ax:%06d",aacx);
        TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"ay:%06d",aacy);
        TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"az:%06d",aacz);
        TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gx:%06d",gyrox);
        TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gy:%06d",gyroy);
        TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);
        sprintf((char*)txt,"gz:%06d",gyroz);
        TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);



    }
}



