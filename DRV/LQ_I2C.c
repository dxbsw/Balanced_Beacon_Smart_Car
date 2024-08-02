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
#include "string.h"
#include "HAL_device.h"
#include "stdio.h"
#include "stdbool.h"
#include "HAL_conf.h"

#define ICM20689_ADDR 0x68
#define MAX(a,b)((a)>(b)?(a):(b))
#define MIN(a,b)((a)<(b)?(a):(b))

typedef struct
{
  //Flag of whether I2C is transmitting
  u8 busy;
  u8 ack;
  //Flag of whether I2C is right
  u8 fault;
  //Flag of I2C transmission direction
  u8 opt;
  //sub address
  u8 sub;
  //number
  u8 cnt;
  //buffer
  u8 *ptr;
  //used to determine if sub addresses need to be sent in interrupt
  u8 sadd;
} i2c_def;
i2c_def i2c;
enum {WR, RD};

void I2C_MasterMode_Init(I2C_TypeDef *I2Cx, u32 uiI2C_speed);
void I2C_Wait(void);
void I2C_Write_Byte(u8 dat);
u8 I2C_Write_Bytes(u8 sub, u8* ptr, u16 cnt);
void I2C_Read_Bytes(void);
void I2C_RcvPacket(u8 sub, u8* ptr, u16 cnt);
void I2C_Check(void);

u8 buffer0[128] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
u8 buffer1[128];

/*******************************************************************************
* @name   : main
* @brief  : I2C poll mode read and write EEPROM
* @param  : None
* @retval : void
*******************************************************************************/
void test_I2C(void)
{
  //Initializes the I2C master mode
  I2C_MasterMode_Init(I2C1, 100000);  
  
  while(1)
  {
  }
}

/*******************************************************************************
* @name              : Initializes the I2Cx master mode
* @brief             : Initializes I2C
* @param I2Cx        : where x can be 1 or 2 to select the I2C peripheral.
* @param uiI2C_speed : I2C speed
* @retval            : void
*******************************************************************************/
void I2C_MasterMode_Init(I2C_TypeDef *I2Cx, u32 uiI2C_speed)
{
  I2C_InitTypeDef I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_4);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_4);
  
  //I2C uses PD10, PD11
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //Need extra pull
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //Configure I2C as master mode
  I2C_InitStructure.I2C_Mode = I2C_Mode_MASTER;
  I2C_InitStructure.I2C_OwnAddress = 0;
  I2C_InitStructure.I2C_Speed = I2C_Speed_STANDARD;
  I2C_InitStructure.I2C_ClockSpeed = uiI2C_speed;
  
  I2C_Init(I2Cx, &I2C_InitStructure);
  I2C_Cmd(I2Cx, ENABLE);  
}


/*******************************************************************************
* @name      : I2C_Wait
* @brief     : Wait for EEPROM getting ready.
* @param dat : None
* @retval    : void
*******************************************************************************/
void I2C_Wait(void)
{
  //eeprom operation interval delay
  u32 i = 10000;
  while(i--);
}

/*******************************************************************************
* @name      : I2C_Write_Byte
* @brief     : Send a byte
* @param dat : data
* @retval    : void
*******************************************************************************/
void I2C_Write_Byte(u8 dat)
{
  I2C_SendData(I2C1, dat);
  //Checks whether transmit FIFO completely empty or not
  while(I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFE) == 0);
}

/*******************************************************************************
* @name      : I2C_Read_Bytes
* @brief     : Receive a byte
* @param ptr : None
* @retval    : void
*******************************************************************************/
void I2C_Read_Bytes(void)
{
  u8 i, flag = 0, _cnt = 0;
  for (i = 0; i < i2c.cnt; i++)
  {
    while(1)
    {
      //Write command is sent when RX FIFO is not full
      if ((I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_TFNF)) && (flag == 0))
      {
        //Configure to read
        I2C_ReadCmd(I2C1);
        _cnt++;
        //When flag is set, receive complete
        if (_cnt == i2c.cnt)
          flag = 1;
      }
      //Check receive FIFO not empty
      if (I2C_GetFlagStatus(I2C1, I2C_STATUS_FLAG_RFNE))
      {
        //read data to i2c.ptr
        i2c.ptr[i] = I2C_ReceiveData(I2C1);
        break;
      }
    }
  }
}

/*******************************************************************************
* @name      : I2C_Write_Bytes
* @brief     : Send bytes
* @param sub : Sub address of EEPROM
* @param ptr : Data in the buffer
* @param cnt : Number of data
* @retval    : The state of this transmission
*******************************************************************************/
u8 I2C_Write_Bytes(u8 sub, u8* ptr, u16 cnt)
{
  //Send sub address
  I2C_Write_Byte(sub);
  while (cnt --)
  {
    I2C_Write_Byte(*ptr);
    //Point to the next data
    ptr++;
  }
  //Stop transmission
  I2C_GenerateSTOP(I2C1, ENABLE);
  //Checks whether stop condition has occurred or not.
  while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);
  i2c.ack = true;
  //I2C operation stops
  i2c.busy = false;
  //Wait for EEPROM getting ready.
  I2C_Wait();
  return true;
}


/*******************************************************************************
* @name      : I2C_RcvPacket
* @brief     : Receive a data packet
* @param sub : Sub address of EEPROM
* @param ptr : Buffer to storage data
* @param cnt : Number of data
* @retval    : void
*******************************************************************************/
void I2C_RcvPacket(u8 sub, u8* ptr, u16 cnt)
{
    //I2C operation starts
    i2c.busy = true;
    i2c.ack = false;
    i2c.sub = sub;
    i2c.ptr = ptr;
    i2c.cnt = cnt;

    //Send sub address
    I2C_Write_Byte(i2c.sub);
    I2C_Read_Bytes();
    I2C_GenerateSTOP(I2C1, ENABLE);
    //Checks whether stop condition has occurred or not.
    while((I2C_GetITStatus(I2C1, I2C_IT_STOP_DET)) == 0);

    i2c.busy = false;
    i2c.ack = true;    
}


