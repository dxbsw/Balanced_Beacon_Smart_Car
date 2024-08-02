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
#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	

#include "stdint.h"
#include "include.h"

//屏幕 硬件/软件SPI切换(1：硬件， 0：软件)
#define use_heard_spi 1

/*******************接口定义******************************/

#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162         //屏幕宽度
#define TFT18H        132         //屏幕高度

#define u16RED		      0xf800    //红色
#define u16GREEN	      0x07e0    //绿色
#define u16BLUE	        0x001f    //蓝色
#define u16PURPLE	      0xf81f    //紫色
#define u16YELLOW	      0xffe0    //黄色
#define u16CYAN	        0x07ff 		//蓝绿色
#define u16ORANGE	      0xfc08    //橘色
#define u16BLACK	      0x0000    //黑色
#define u16WHITE	      0xffff    //白色
void TFTSPI_Init(unsigned short type);
void TFTSPI_Write_Cmd(unsigned short cmd);
void TFTSPI_Write_Byte(unsigned short dat);
void TFTSPI_Write_Word(unsigned short dat);
void TFTSPI_Set_Pos(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye);
void TFTSPI_Fill_Area(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color);
void TFTSPI_CLS(unsigned short color);
void TFTSPI_Draw_Part(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);
void TFTSPI_Draw_Rectangle(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);
void TFTSPI_Draw_Circle(unsigned short x,unsigned short y,unsigned short r,unsigned short color_dat);
void TFTSPI_Draw_Line(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,unsigned short color_dat);
void TFTSPI_Draw_Dot(unsigned short x,unsigned short y,unsigned short color_dat);
void TFTSPI_Show_Pic(unsigned short xs,unsigned short ys,unsigned short xe,unsigned short ye,const unsigned char *ppic);
void TFTSPI_P6X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X8Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P8X16Str(unsigned short x, unsigned short y, char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_P16x16Str(unsigned short x, unsigned short y, unsigned char *s_dat,unsigned short word_color,unsigned short back_color);
void TFTSPI_Show_Pic2(unsigned short xs,unsigned short ys,unsigned short w,unsigned short h,const unsigned char *ppic) ;
void TFTSPI_Show_Battery_Icon(void);
void TFTSPI_Show_Title(void);
void TFTSPI_Show_Logo(unsigned short xs,unsigned short ys);
void TFTSPI_Road( unsigned short wide_start, unsigned short high_start,unsigned short high, unsigned short wide, unsigned char *Pixle);
void TFTSPI_BinRoad( unsigned short wide_start, unsigned short high_start,unsigned short high, unsigned short wide, unsigned char *Pixle);
void TFTSPI_BinRoad2(unsigned short wide_start, unsigned short high_start, unsigned short high, unsigned short wide);
void Test_TFT18(void);


#endif /*SGP18T_ILI9163B.h*/
