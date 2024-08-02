#include "SIR_BELL.h"

/*
【函数名】 void SIR_BELL_Init(  )
【功  能】 蜂鸣器初始化
【参数值】 pin ：gpio管脚
【返回值】 无 
【最后更新】 2021年1月22日 
【软件版本】 V1.0
【调用样例】 SIR_BELL_Init()
*/
void SIR_BELL_Init()
{
  PIN_InitConfig(PD6, GPO, 1,GPIO_Mode_Out_PP);
}
