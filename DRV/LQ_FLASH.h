/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@编    写：龙邱科技
@E-mail  ：chiusir@163.com
@编译IDE：IAR7.8 KEIL5.24及以上版本
@使用平台：北京龙邱智能科技F3277核心板和母板
@SYS  PLL：120MHz 频率太高可能无法启动system_mm32f327x.c
@最后更新：2022年02月22日，持续更新，请关注最新版！
@功能介绍：
@相关信息参考下列地址
@网    站：http://www.lqist.cn
@淘宝店铺：http://longqiu.taobao.com
@
@软件版本：V1.0 版权所有，单位使用请先联系授权
@程序配套视频地址：https://space.bilibili.com/95313236
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_FLASH_h
#define _LQ_FLASH_h
#include <stdint.h>

#define FLASH_SIZE 512 	 		  //FLASH容量大小(单位为K)

#define FLASH_BASE_ADDR             (0x08000000)        //FALSH起始地址
#define FLASH_PAGE_SIZE             1024                //每页1K字节
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4) //每扇区4K字节

//共16扇区，每扇区4K。
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define PAGE_WRITE_START_ADDR  ((uint32_t)0x0800F000) //从60K开始
#define PAGE_WRITE_END_ADDR    ((uint32_t)0x08010000) //63K结束

#define FLASH_PAGES_TO_BE_PROTECTED FLASH_WRProt_Pages60to63

typedef enum
{
  FLASH_PAGE_0,
  FLASH_PAGE_1,
  FLASH_PAGE_2,
  FLASH_PAGE_3,
}FLASH_PAGE_enum;

u16   LQFLASH_ReadByte(u32 faddr);		                        //读出一个字节 
u32   LQFLASH_ReadLenByte(u32 ReadAddr,u16 Len);			//指定地址开始读取指定长度数据
void LQFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//指定地址开始写入指定长度的数据
void LQFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//从指定地址开始写入指定长度的数据
void LQFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//从指定地址开始读出指定长度的数据
void pid_TFT_show(char i); //屏幕显示

void Test_EEPROM(void);

#endif
