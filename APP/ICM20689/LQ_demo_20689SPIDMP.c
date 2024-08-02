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
本程序测试MPU6050DMP功能，配置匿名地面站4.34显示波形
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"

extern short   gyro[3], accel[3], sensors;
extern float   Pitch, Roll; 

void Test_LQDMP(void)
{
  char  txt[10];
  u8 tm[2];
  u8 res;
  
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏  
  ICM_SPI_Init(); 
  TFTSPI_P8X16Str(2,0,"LQ DMP Test", u16RED, u16BLACK);
  UART_PutStr(UART1, "LQ DMP Test\r\n");
  delayms(100);  
  res=ICM_Read_Byte(WHO_AM_I);                       //读取ICM20689的ID  
  sprintf(txt,"ID:0x%02X",res);  
  TFTSPI_P8X16Str(0, 9, txt, u16PURPLE, u16BLACK);   // 字符串显示
  LQ_SPI_Read(WHO_AM_I, 1, tm);  
  sprintf(txt,"ID:0x%02X",tm[0]);  
  TFTSPI_P8X16Str(8, 9, txt, u16PURPLE, u16BLACK);   // 字符串显示
  LQ_DMP_Init();
  delayms(100);
  while(1)
  {       
    LQ_DMP_Read();    
    
    sprintf((char*)txt,"Pitch:%.2f", Pitch);
    TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"Roll:%.2f", Roll);
    TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);
    
    sprintf((char*)txt,"ax:%06d",accel[0]);
    TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"ay:%06d",accel[1]);
    TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"az:%06d",accel[2]);
    TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gx:%06d",gyro[0]);
    TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gy:%06d",gyro[1]);
    TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gz:%06d",gyro[2]);
    
    //上位机
    ANO_DT_send_int16((short)Pitch, (short)Roll, 0, 0, 0, 0, 0, 0 );
  }
}

