
#include "include.h"


static int key_flag = 0;
/*************************************************************************
*  函数名称：void GPIO_KEY_Init(void)
*  功能说明：GPIO初始化函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void GPIO_KEY_Init(void)
{
  PIN_InitConfig(PD0, GPI, 1,GPIO_Mode_IPU);
  PIN_InitConfig(PD1, GPI, 1,GPIO_Mode_IPU);
  PIN_InitConfig(PD2, GPI, 1,GPIO_Mode_IPU);
  PIN_InitConfig(PD3, GPI, 1,GPIO_Mode_IPU);    
  PIN_InitConfig(PD4, GPI, 1,GPIO_Mode_IPU);
  PIN_InitConfig(PA2, GPI, 1,GPIO_Mode_IPU);    
}

/*************************************************************************
*  函数名称：int KEY_Read_All(void)
*  功能说明：读取按键状态
*  参数说明：
*  函数返回：按键状态，0/1
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
int KEY_Read_All(void)
{
  static int s1=0,s2 =0,s3=0;
  static int Ls1=0,Ls2=0,Ls3=0;

  Ls1=s1,Ls2=s2,Ls3=s3;
  s1 = PIN_Read(KEY0p);//上面按钮
  s2 = PIN_Read(KEY1p);//中间按钮
  s3 = PIN_Read(KEY2p);//下面按钮
  
    if(Ls1==1&&s1==0)//上面按钮
  {
   delayms(10);
   if(s1==0)
   {
    key_flag=1;//上面
   }
  }
  else if(Ls2==1&&s2==0)//中间按钮
  {
   delayms(10);
   if(s2 == 0)
   {
    key_flag=2;//中间
   }
  }
  else if(Ls3==1&&s3==0)//下面按钮
  {
   delayms(10);
   if(s3 == 0)
   {
    key_flag=3;//下面
   }
  }
  return key_flag;
}


/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：u8 KEY_Scan(u8 mode)
@功能说明：按键处理函数
@参数说明：mode：0不支持连续按；1支持连续按
@函数返回：无
@修改时间：2022/02/24
@备    注：按键状态彩屏显示
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
u8 KEY_Scan(u8 mode)
{	 
  static u8 key_up=1;//按键松开标志位
  if(mode)key_up=1;  //支持连按	  
  if(key_up&&(KEY_Read(KEY0)==0||KEY_Read(KEY1)==0||KEY_Read(KEY2)==0))
  {
    delayms(10); 
    key_up=0;
    if(KEY_Read(KEY0)==0)return 1;
    else if(KEY_Read(KEY1)==0)return 2;
    else if(KEY_Read(KEY2)==0)return 3;
    else if(KEY_Read(KEY0)==0&&KEY_Read(KEY1)==0)return 4;
    else if(KEY_Read(KEY2)==0&&KEY_Read(KEY1)==0)return 5;

  }else if(KEY_Read(KEY0)==1&&KEY_Read(KEY1)==1&&KEY_Read(KEY2)==1)key_up=1; 
  
  return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@函数名称：unsigned char KEY_Read(KEYn_e KEYno)
@功能说明：读取按键状态
@参数说明：KEYn_e KEYno按键编号
@函数返回：按键状态，0/1
@修改时间：2022/02/24
@备    注：
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
unsigned char KEY_Read(KEYn_e KEYno)
{
  switch(KEYno)
  {
  case KEY0:
    return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4 );//母板上按键0
    //break;    
  case KEY1:
    return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2 );//母板上按键1
    //break;   
  case KEY2:
    return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);//母板上按键2
    //break;    
  case DSW0:
    return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0 );//母板上拨码开关0
    //break;
  case DSW1:
    return GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1 );//母板上拨码开关1
    //break;
  default:
    return 0XFF;
  }
  //return 0;
}

