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
#include <LQ_GPIO.h>
#include "stdio.h"

//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
u8 RX2CHAR=0;
extern signed short realSpeed3, realSpeed4;
void UART1_IRQHandler(void)
{
  //接收中断
  if(UART_GetFlagStatus(UART1, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART1,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART1,UART_GetChar(UART1));
  }
}

void UART2_IRQHandler(void)
{
  //接收中断
  if(UART_GetFlagStatus(UART2, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART2,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART2,UART_GetChar(UART2));
    
  }
}
void UART3_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART3, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART3,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART3,UART_GetChar(UART3));
    
  }
}
void UART4_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART4, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART4,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART4,UART_GetChar(UART4));
    
  }
}
void UART5_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART5, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART5,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART5,UART_GetChar(UART5));
    
  }
}
void UART6_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART6, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART6,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART6,UART_GetChar(UART6));
    
  }
}
void UART7_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART7, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART7,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART7,UART_GetChar(UART7));
    
  }
}
void UART8_IRQHandler(void)
{  
  if(UART_GetFlagStatus(UART8, UART_IT_RXIEN)  != RESET)
  {
    UART_ClearITPendingBit(UART8,UART_IT_RXIEN);
    /* 用户代码 */
    UART_PutChar(UART8,UART_GetChar(UART8));
    
  }
}
/*************************************************************************
*  函数名称：void UART1_InitConfig(unsigned long baudrate)
*  功能说明：串口模块初始化
*  参数说明：
* @param    RxPin   ： 串口接收管脚
* @param    TxPin   ： 串口发送管脚
* @param    baudrate： 波特率
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：void UART1_InitConfig(115200);   //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
*************************************************************************/
void UART_PinConfig(UART_TX_e tx_pin, UART_RX_e rx_pin)
{
  PIN_InitConfig((GPIO_Name_t)(tx_pin &0xFF), GPO, 0, GPIO_Mode_AF_PP);  
  GPIO_PinAFConfig((GPIO_TypeDef *)(PORTX[(tx_pin&0x00F0) >> 4]),  tx_pin & 0x0F, (u16)tx_pin >> 12);
  PIN_InitConfig((GPIO_Name_t)(rx_pin &0xFF), GPI, 0, GPIO_Mode_IN_FLOATING);  
  GPIO_PinAFConfig((GPIO_TypeDef *)(PORTX[(rx_pin&0x00F0) >> 4]),  rx_pin & 0x0F, (u16)rx_pin >> 12);  
}
/*************************************************************************
*  函数名称：void UART_InitConfig(UART_TypeDef* UARTx,unsigned long baudrate,UART_TX_e tx_pin, UART_RX_erx_pin)
*  功能说明：串口模块初始化
*  参数说明：
* @param    RxPin   ： 串口接收管脚
* @param    TxPin   ： 串口发送管脚
* @param    baudrate： 波特率
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_InitConfig(USAR1,9600,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
*************************************************************************/
void UART_InitConfig(UART_TypeDef* UARTx,unsigned long baudrate,UART_TX_e tx_pin, UART_RX_e rx_pin)
{
  UART_InitTypeDef UART_InitStructure;  
  
  //串口总线使能
  if(UART1 == UARTx) RCC_APB2PeriphClockCmd(RCC_APB2Periph_UART1, ENABLE); //APB2
  else if(UART2 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART2, ENABLE);  
  else if(UART3 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART3, ENABLE); 
  else if(UART4 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART4, ENABLE); 
  else if(UART5 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART5, ENABLE); 
  else if(UART6 == UARTx) RCC_APB2PeriphClockCmd(RCC_APB2ENR_UART6, ENABLE);    //APB2
  else if(UART7 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART7, ENABLE);
  else if(UART8 == UARTx) RCC_APB1PeriphClockCmd(RCC_APB1ENR_UART8, ENABLE); 	
  //引脚初始化
  UART_PinConfig(tx_pin, rx_pin);
  
  //串口参数配置
  UART_InitStructure.UART_BaudRate = baudrate;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
  UART_InitStructure.UART_Mode = UART_Mode_Tx | UART_Mode_Rx;
  
  //使能串口
  UART_RX_IRQ_Config(UARTx,ENABLE);
  UART_Init(UARTx, &UART_InitStructure);
  UART_Cmd(UARTx, ENABLE);
  
  
}

/*************************************************************************
*  函数名称：void UART_RX_IRQ_Config(UART_NAME_t UART_x,u8 status)
*  功能说明：开启串口接收中断
*  参数说明：
* @param    UART_x       串口通道
* @param    status      使能或禁止
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_RX_IRQ_Config(UART_1, ENABLE);  //开启串口1接收中断
*************************************************************************/
void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_RXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status); 
}

/*************************************************************************
*  函数名称：void UART_RX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
*  功能说明：开启串口接收中断
*  参数说明：
* @param    UART_x       串口通道
* @param    status      使能或禁止
*  函数返回：字节
*  修改时间：2020年3月10日
*  备    注：UART_RX_IRQ_Config(UART_1, ENABLE);  //开启串口1接收中断
*************************************************************************/
void UART_TX_IRQ_Config(UART_TypeDef* UARTx,u8 status)
{
  UART_ITConfig(UARTx, UART_IT_TXIEN, ENABLE);
  if(UART1 == UARTx) nvic_init(UART1_IRQn, 0, 0, status);
  else if(UART2 == UARTx) nvic_init(UART2_IRQn, 0, 0, status);  
}
/*************************************************************************
*  函数名称：void UART_PutChar(UART_t  uratn, char ch)
*  功能说明：UART发送字节函数,使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ ch    ： 要打印的字符
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutChar(UART0, 'a');  //打印字符a
*************************************************************************/
void UART_PutChar(UART_TypeDef* UARTx,char ch)
{
  UART_SendData(UARTx, ch);
  while (UART_GetFlagStatus(UARTx, UART_FLAG_TXEPT) == RESET);
}

/*************************************************************************
*  函数名称：void UART_PutStr(UART_t  uratn, char *str)
*  功能说明：UART发送字符串函数(遇 NULL 停止发送),使用前请先初始化对应串口
*  参数说明：uratn ： UART0 - UART3/ str   ： 要打印的字符串地址
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：UART_PutStr(UART3, "123456789"); //发送9个字节
*************************************************************************/
void UART_PutStr(UART_TypeDef* UARTx,char *str)
{
  while(*str)
  {
    UART_PutChar(UARTx,*str++);
  }
}
//匿名地面站上位机接口函数
void UART_PutBuff(UART_TypeDef* UARTx, u8 *buff, u16 len)
{
  while(len--)																				
  {
    UARTx->TDR = *buff++;														// 放入缓冲区
    while(!(UARTx->CSR & UART_CSR_TXC));						// 等待发送完成		
  }
}
/*************************************************************************
*  函数名称：char UART_GetChar(UART_t  uratn)
*  功能说明：UART读取字节 使用前请先初始化对应串口 调用前请确认有接收到数据 否则会等待数据到来
*  参数说明：uratn ： UART0 - UART3
*  函数返回：读取字节
*  修改时间：2020年3月10日
*  备    注：UART_GetChar(UART3); //接收一个字符
*************************************************************************/
char UART_GetChar(UART_TypeDef* UARTx)
{  
  return (UART_ReceiveData(UARTx));
}


/*******************************************************************************
此函数为printf函数接口，设置步骤为：
project-->options-->general options-->library configurations-->library下面选择full

也可以对应修改后使用别的串口。
*******************************************************************************/
int fputc(int ch, FILE *f)
{  
  UART_SendData(UART1, ch);
  while (UART_GetFlagStatus(UART1, UART_FLAG_TXEPT) == RESET);  
  
  return ch;  
} 

/*************************************************************************
*  函数名称：void Test_Bluetooth(void);
*  功能说明：UART测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void Test_Bluetooth(void)
{  
  UART_InitConfig(UART1,115200,UART1_TX_A9,UART1_RX_A10); //初始化串口1 波特率 115200 无奇偶校验 1停止位 使用管脚
  UART_PutStr(UART1," UART1 LongQiu \r\n"); //发送字符串到上位机
  while(1)
  {
    UART_PutChar(UART1,'L');                     //发送 字节到UART口
    delayms(500);                                  //延时等待
    UART_PutChar(UART1,'Q');                      //发送 字节到UART口
    
    UART_PutStr(UART1,"UART1 LongQiu \r\n");      //发送字符串到上位机
    
    LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
    delayms(500);              //延时等待
  }
}

//
