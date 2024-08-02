/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 7.19 晚 最快37，平均45前
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "include.h"
  int main(void)
{
  // 系统及延时函数初始化
  LQ_Init3227();	    
  //LED KEY初始化
  GPIO_LED_Init();
  SIR_BELL_Init();

  
  //TFT屏幕初始化
  TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕

  //主要运行程序
  Balance_CAR ();

  while(1)
  {
    //Test_ICM20689();
  }
}




