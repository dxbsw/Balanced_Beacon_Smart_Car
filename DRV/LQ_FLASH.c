/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编    写：龙邱科技
@E-mail  ：chiusir@163.com
@编译IDE：IAR7.8 KEIL5.24及以上版本
@使用平台：北京龙邱智能科技F3277核心板和母板
@SYS  PLL：120MHz 频率太高可能无法启动system_mm32f327x.c
@最后更新：2022年02月22日，持续更新，请关注最新版！
@功能介绍：flash
@相关信息参考下列地址
@网    站：http://www.lqist.cn
@淘宝店铺：http://longqiu.taobao.com
@
@软件版本：V1.0 版权所有，单位使用请先联系授权
@程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
#include "HAL_conf.h"
#include "HAL_device.h"
#include "HAL_flash.h"
#include "LQ_FLASH.h"
void pid(pid_param_t * pid,int i,float numb);

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】u16 LQFLASH_ReadByte(u32 faddr)
【功  能】读取指定地址的半字(16位数据)
【参数值】faddr:读地址(此地址必须为2的倍数!!)
【返回值】对应地址的数据.
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】buf=LQFLASH_ReadByte(faddr)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u16 LQFLASH_ReadByte(u32 faddr)
{
  return *(vu16*)faddr; 
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LQFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
【功  能】不检查的写入(16位数据)
【参数值】riteAddr:起始地址
【参数值】pBuffer:数据指针
【参数值】NumToWrite:半字(16位)数   
【返回值】无
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】buf=LQFLASH_ReadByte(faddr)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
  u16 i;
  for(i=0;i<NumToWrite;i++)
  {
    FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
    WriteAddr+=2;//地址增加2.
  }  
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LQFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
【功  能】从指定地址开始写入指定长度的数据
【参数值】WriteAddr:起始地址(此地址必须为2的倍数!!)
【参数值】pBuffer:数据指针
【参数值】NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
【返回值】无
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】buf=LQFLASH_ReadByte(faddr)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#if LQ_FLASH_SIZE<256
#define LQ_SECTOR_SIZE 1024 //字节
#else 
#define LQ_SECTOR_SIZE	2048
#endif		 
u16 LQFLASH_BUF[LQ_SECTOR_SIZE/2];//最多是2K字节

void LQFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
  u32 secpos;	   //扇区地址
  u16 secoff;	   //扇区内偏移地址(16位字计算)
  u16 secremain;   //扇区内剩余地址(16位字计算)	   
  u16 i;    
  u32 offaddr;     //去掉0X08000000后的地址
  if(WriteAddr<FLASH_BASE_ADDR||(WriteAddr>=(FLASH_BASE_ADDR+1024*FLASH_SIZE)))return;//非法地址
  FLASH_Unlock();						//解锁
  offaddr=WriteAddr-FLASH_BASE_ADDR;		//实际偏移地址.
  secpos=offaddr/LQ_SECTOR_SIZE;			//扇区地址  0~127 for LQF103RBT6
  secoff=(offaddr%LQ_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
  secremain=LQ_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
  if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
  while(1) 
  {	
    LQFLASH_Read(secpos*LQ_SECTOR_SIZE+FLASH_BASE_ADDR,LQFLASH_BUF,LQ_SECTOR_SIZE/2);//读出整个扇区的内容
    for(i=0;i<secremain;i++)//校验数据
    {
      if(LQFLASH_BUF[secoff+i]!=0XFFFF) break;//需要擦除  	  
    }
    if(i<secremain)//需要擦除
    {
      FLASH_ErasePage(secpos*LQ_SECTOR_SIZE+FLASH_BASE_ADDR);//擦除这个扇区
      for(i=0;i<secremain;i++)//复制
      {
        LQFLASH_BUF[i+secoff]=pBuffer[i];	  
      }
      LQFLASH_Write_NoCheck(secpos*LQ_SECTOR_SIZE+FLASH_BASE_ADDR,LQFLASH_BUF,LQ_SECTOR_SIZE/2);//写入整个扇区  
    }else LQFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
    if(NumToWrite==secremain)break;//写入结束了
    else//写入未结束
    {
      secpos++;				//扇区地址增1
      secoff=0;				//偏移位置为0 	 
      pBuffer+=secremain;  	//指针偏移
      WriteAddr+=secremain;	//写地址偏移	   
      NumToWrite-=secremain;	//字节(16位)数递减
      if(NumToWrite>(LQ_SECTOR_SIZE/2))secremain=LQ_SECTOR_SIZE/2;//下一个扇区还是写不完
      else secremain=NumToWrite;//下一个扇区可以写完了
    }	 
  };	
  FLASH_Lock();//上锁
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void LQFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
【功  能】从指定地址开始读出指定长度的数据
【参数值】ReadAddr:起始地址
【参数值】pBuffer:数据指针
【参数值】NumToWrite:半字(16位)数
【返回值】无
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】buf=LQFLASH_ReadByte(faddr)
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void LQFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
  u16 i;
  for(i=0;i<NumToRead;i++)
  {
    pBuffer[i]=LQFLASH_ReadByte(ReadAddr);//读取2个字节.
    ReadAddr+=2;//偏移2个字节.	
  }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【函数名】void Test_EEPROM(void)
【功  能】测试FLASH读写，通过按键进行操作
【参数值】无
【返回值】无
【作  者】L Q
【最后更新】2021年1月22日 
【软件版本】V1.0
【调用样例】
---------------------------------------------------------------
//按下母板上KEY0按键，写入当前变量到指定地址
//按下母板上KEY2按键，读取指定地址数据到屏幕
//复位单片机后，如果仍然能读取到原来写入的数据，说明保存成功
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

float TEXT_Buffer[15]={-4.8,0,-1.55,-355,0,-100,0.35,0,0,0.4,0,0,120,0,80};
#define SIZE sizeof(TEXT_Buffer)	 	//数组长度
#define FLASH_SAVE_ADDR  0X08070000 		//设置FLASH 保存地址(必须为偶数)
float datatemp[SIZE];
u8 cnt=1,pid_flag=0;
u8 key; 
char txt[16];
extern float PID_k=0.55;

extern pid_param_t Gyro_PID,Angle_PID,BalSpeed_PID,BalDirgyro_PID,BalDir_PID;

void Test_EEPROM(void)
{	
  
  LQFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);   //读取指定地址数据
  TEXT_Buffer[0] = datatemp[0];         //-3.8
  TEXT_Buffer[1] = datatemp[1];  
  TEXT_Buffer[2] = datatemp[2];          //-1
  TEXT_Buffer[3] = datatemp[3];         //-3.8
  TEXT_Buffer[4] = datatemp[4];  
  TEXT_Buffer[5] = datatemp[5];          //-1
  TEXT_Buffer[6] = datatemp[6];         //-3.8
  TEXT_Buffer[7] = datatemp[6];  
  TEXT_Buffer[8] = datatemp[8];          //-1
  
  TEXT_Buffer[9] = datatemp[9];         //-3.8
  TEXT_Buffer[10] = datatemp[10];  
  TEXT_Buffer[11] = datatemp[11];          //-1
  TEXT_Buffer[12] = datatemp[12];         //-3.8
  TEXT_Buffer[13] = datatemp[13];  
  TEXT_Buffer[14] = datatemp[14];          //-1
  
  Gyro_PID.kp = TEXT_Buffer[0]*PID_k;         //-3.8
  Gyro_PID.ki = TEXT_Buffer[1]*PID_k;  
  Gyro_PID.kd = TEXT_Buffer[2]*PID_k;          //-1
  
  Angle_PID.kp = TEXT_Buffer[3]*PID_k;         //-3.8
  Angle_PID.ki = TEXT_Buffer[4]*PID_k;  
  Angle_PID.kd = TEXT_Buffer[5]*PID_k;          //-1
  
  BalSpeed_PID.kp = TEXT_Buffer[6];         //-3.8
  BalSpeed_PID.ki = TEXT_Buffer[6]/150;  
  BalSpeed_PID.kd = TEXT_Buffer[8];          //-1
  
  BalDirgyro_PID.kp = TEXT_Buffer[9];       
  BalDirgyro_PID.ki = TEXT_Buffer[10];
  BalDirgyro_PID.kd = TEXT_Buffer[11];
  //位置
  BalDir_PID.kp = TEXT_Buffer[12];           //方向外环参数赋值  方向偏差
  BalDir_PID.ki = TEXT_Buffer[13];
  BalDir_PID.kd = TEXT_Buffer[14];
  
  
  if(KEY_Read(DSW0)==0&&KEY_Read(DSW1)==0)
  {
    TFTSPI_CLS(u16WHITE);
    pid(&BalDirgyro_PID,3,0.01);//pid
    pid_TFT_show(3);
    sprintf(txt, "error:%.2f", Dir_Error);
    TFTSPI_P8X8Str(0, 10, txt, u16RED, u16BLUE);
    
  }
  
  if(KEY_Read(DSW0)==0&&KEY_Read(DSW1)==1)
  {
    pid(&BalDir_PID,4,1);//pid
    pid_TFT_show(4);
    sprintf(txt, "error:%.2f", Dir_Error);
    TFTSPI_P8X8Str(0, 10, txt, u16RED, u16BLUE);
    
  }
  
  if(KEY_Read(DSW0)==1&&KEY_Read(DSW1)==1)
  {
    pid(&BalSpeed_PID,2,0.01);//pid
    pid_TFT_show(2);
    sprintf(txt, "error:%.2f", Dir_Error);
    TFTSPI_P8X8Str(0, 10, txt, u16RED, u16BLUE);
  }
  
  if(KEY_Read(DSW0)==1&&KEY_Read(DSW1)==0)
  {
    
    sprintf((char*)txt,"Roll:%.2f", Roll);         // 显示小车平衡中值
    TFTSPI_P8X8Str(0,9,txt,u16RED,u16BLUE);
    sprintf(txt, "error:%.2f", Dir_Error);
    TFTSPI_P8X8Str(0, 10, txt, u16RED, u16BLUE);
      
      // 电机和编码器参数显示
//      sprintf(txt, "M1:%04d, M2:%04d ", MotorDuty1, MotorDuty2);
//      TFTSPI_P8X8Str(0, 11, txt, u16RED, u16BLUE);     // 电机1-2数值
//      sprintf(txt, "E1:%04d, E2:%04d ", ECPULSE1, ECPULSE2);
//      TFTSPI_P8X8Str(0, 12, txt, u16RED, u16BLUE);     // 编码器1-2数值
//      sprintf(txt, "Dirgyro:%.2f", BalDirgyro_out);
//      TFTSPI_P8X8Str(0, 13, txt, u16RED, u16BLUE);     // IR灯的均值和期望差值
//      sprintf(txt, "Dirgyro:%.2f", BALANCE_OUT);
//      TFTSPI_P8X8Str(0, 14, txt, u16RED, u16BLUE);     // IR灯的均值和期望差值
      
    }
    

}
/*pid参数存入flash*/
void pid(pid_param_t * pid,int i,float numb)
{
  key=KEY_Scan(1);
  switch(key)
  {
  case 1:
    {
      switch(cnt)
      {
      case 1: TEXT_Buffer[0+i*3]=pid->kp+numb;break;
      case 2: TEXT_Buffer[1+i*3]=pid->ki+numb;break;
      case 3: TEXT_Buffer[2+i*3]=pid->kd+numb;break;
      }
      LQFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);//写入当前变量到指定地址	
      LQFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);   //读取指定地址数据
      pid->kp = datatemp[0+i*3];         //-3.8
      pid->ki = datatemp[1+i*3];  
      pid->kd = datatemp[2+i*3];          //-1
    }break;
    
  case 2:
    {
      switch(cnt)
      {
      case 1: TEXT_Buffer[0+i*3]=pid->kp-numb;break;
      case 2: TEXT_Buffer[1+i*3]=pid->ki-numb;break;
      case 3: TEXT_Buffer[2+i*3]=pid->kd-numb;break;
      }                               //变动数值，以便观察读写是否成功
      LQFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);//写入当前变量到指定地址	
      LQFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE);   //读取指定地址数据
      pid->kp = datatemp[0+i*3];         //-3.8
      pid->ki = datatemp[1+i*3];  
      pid->kd = datatemp[2+i*3];          //-1
    }break;
    
  case 3:
    {
      cnt++;
      delayms(100);
      if(cnt>3)cnt=1;
    }break;
  }
}

void pid_TFT_show(char i)//屏幕显示
{
  if(i==0)
  {
    switch(cnt)
    {
    case 1:
      sprintf(txt,"Gyro_P:%.2f",Gyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_I:%.2f",Gyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_D:%.2f",Gyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 2:
      sprintf(txt,"Gyro_P:%.2f",Gyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_I:%.2f",Gyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_D:%.2f",Gyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 3:
      sprintf(txt,"Gyro_P:%.2f",Gyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_I:%.2f",Gyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Gyro_D:%.2f",Gyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      break;
    }
  }
  
  if(i==1)
  {
    switch(cnt)
    {
    case 1:
      sprintf(txt,"Angle_P:%.2f",Angle_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_I:%.2f",Angle_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_D:%.2f",Angle_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 2:
      sprintf(txt,"Angle_P:%.2f",Angle_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_I:%.2f",Angle_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_D:%.2f",Angle_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 3:
      sprintf(txt,"Angle_P:%.2f",Angle_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_I:%.2f",Angle_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Angle_D:%.2f",Angle_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      break;
    }
  }
  
  if(i==2)
  {
    switch(cnt)
    {
    case 1:
      sprintf(txt,"Speed_P:%.2f",BalSpeed_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_I:%.3f",BalSpeed_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_D:%.2f",BalSpeed_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 2:
      sprintf(txt,"Speed_P:%.2f",BalSpeed_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_I:%.3f",BalSpeed_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_D:%.2f",BalSpeed_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 3:
      sprintf(txt,"Speed_P:%.2f",BalSpeed_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_I:%.3f",BalSpeed_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Speed_D:%.2f",BalSpeed_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      break;
    }
  }
  
  if(i==3)
  {
    switch(cnt)
    {
    case 1:
      sprintf(txt,"Dirgyro_P:%.2f",BalDirgyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_I:%.2f",BalDirgyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_D:%.2f",BalDirgyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 2:
      sprintf(txt,"Dirgyro_P:%.2f",BalDirgyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_I:%.2f",BalDirgyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_D:%.2f",BalDirgyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 3:
      sprintf(txt,"Dirgyro_P:%.2f",BalDirgyro_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_I:%.2f",BalDirgyro_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"Dirgyro_D:%.2f",BalDirgyro_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      break;
    }
  }
  
  if(i==4)
  {
    switch(cnt)
    {
    case 1:
      sprintf(txt,"BalDir_P:%.2f",BalDir_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_I:%.2f",BalDir_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_D:%.2f",BalDir_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 2:
      sprintf(txt,"BalDir_P:%.2f",BalDir_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_I:%.2f",BalDir_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_D:%.2f",BalDir_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16RED,u16BLUE);             //屏幕显示出来
      break;
    case 3:
      sprintf(txt,"BalDir_P:%.2f",BalDir_PID.kp);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,1,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_I:%.2f",BalDir_PID.ki);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,3,txt,u16RED,u16BLUE);             //屏幕显示出来
      sprintf(txt,"BalDir_D:%.2f",BalDir_PID.kd);                  //转换为字符串，以便屏幕显示出来
      TFTSPI_P8X8Str(1,5,txt,u16GREEN,u16BLUE);             //屏幕显示出来
      break;
    }
  }
}
