
#include "include.h"


//直立环参数
extern pid_param_t Gyro_PID,Angle_PID,BalSpeed_PID,BalDirgyro_PID,BalDir_PID;

extern int16_t aacx, aacy, aacz;
extern int16_t gyrox, gyroy, gyroz;

// 电感归一化偏移量
  
extern float Dir_Error;
extern uint8_t labelcount;
int bell_flag=1;
extern int16_t targetSpeed1;

unsigned short Threshold = 0;
volatile uint8_t dotcnt=0;
volatile uint8_t dotlie[10000];
volatile uint8_t dothang[10000];
volatile int sumlie=0,sumhang=0;
volatile uint8_t batchargeflg = 1;
volatile uint8_t beaconFlashCnt= 1;

uint8_t shang=0,xia=0,zuo=0,you=0;

/*************************************************************************
*  函数名称：void Balance_CAR(void)
*  功能说明：直立车直立控制
-->串级PID控制直立，角速度环为内环，角度环为外环
*  参数说明：无
*  函数返回：无
*  修改时间：2021年04月01日
*  备    注：直立需要时序控制，需将下面的定时器中断函数覆盖原定时器中断函数
*************************************************************************/
void Balance_CAR (void)
{ 
  uint16_t cnt=0;
  char  txt[10];
  CAMERA_Init(100);          // 摄像头初始化 
  EncInit();            // 初始化两个编码器  编码器2和编码器3
  MotorInit(12500);     // 电机驱动的PWM为12.5Khz  使用PC6、PC7控制电机1，PC8、PC9控制电机2
  TFTSPI_Init(0);        //LCD初始化  0:横屏  1：竖屏
  SOFT_IIC_Init();            //IIC初始化   6050陀螺仪初始化
  ICM20689_Init();            //icm20689初始化 SPI CS-PE10  SCK-PE12 MISO-PE13 MOSI-PE14 
  LQ_DMP_Init();            //MPU6050DMP();   
  GPIO_KEY_Init();  
  PidInit(&Gyro_PID);     //直立内环参数初始化
  PidInit(&Angle_PID);    //直立外环参数初始化
  PidInit(&BalSpeed_PID);    //速度环参数初始化
  PidInit(&BalDirgyro_PID);   //方向内环参数初始化
  PidInit(&BalDir_PID);       //方向外环参数初始化
  
  PID_init();//变积分
  
  //PID 参数需要自行调节 
  //位置
  //直立内环参数赋值  角速度
    float PID_k=0.8;//调整直立软硬程度，调参时默认为1
    
  Gyro_PID.kp = -3*PID_k;
  Gyro_PID.ki = -0;          
  Gyro_PID.kd = -2.05*PID_k;
  //位置
  //直立外环参数赋值   角度
  Angle_PID.kp = -296.8*PID_k;
  Angle_PID.ki = -0;         
  Angle_PID.kd = -82*PID_k;
  

  //增量
  //速度环参数赋值
                                    
  BalSpeed_PID.kp = 0.08;
  BalSpeed_PID.ki = 0.003;
  BalSpeed_PID.kd = 0;
 
    //位置
  //方向内环参数赋值  角速度
  BalDirgyro_PID.kp = 0.38;       
  BalDirgyro_PID.ki = 0;
  BalDirgyro_PID.kd = 0.015;
  //位置
  BalDir_PID.kp = 130;           //方向外环参数赋值  方向偏差
  BalDir_PID.ki = 0;
  BalDir_PID.kd = 60;
  Camera_Flag = 1;
  delayms(100);
  TIMER_InitConfig(TIMER_6, 5);// 读取PID定时中断5ms
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕  
  int eyes;
  while (1)
  {
    // 信标灯识别
    if (Camera_Flag == 1)
    {
      // 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 
      Camera_Flag = 0;          
      // 提取部分使用的数据
      Get_Use_Image();    
      // 二值化 //
      //阈值选择，1大津法(不好用)，2亮度均值，3固定阈值
      Get_Bin_Image(1);
      
      
//图像处理修改处
		
//环境复杂时使用,三个必须同时使用
//      ConnectedComponentLabeling((unsigned char *) Bin_Image ,labelBuffer);   //连通域计算
//      quanzhong_light_panduan();//通过连通域得到的图像进行判断
//      quanzhong_light();//计算目标位置信息
		
// 判断信标灯位置(简单版)，环境良好时使用
      Seek_Beacon();

    }
    //此处二选一
    //Test_EEPROM();  //flash按键修改函数
    TFT_show();   //tft显示函数
  }
}

  
// TFT屏幕信息显示
void TFT_show(void)//屏幕显示
{
  static int last_KeyValue,Now_KeyValue;
  uint16_t cnt=0;
  char  txt[64];
  last_KeyValue = Now_KeyValue;
  Now_KeyValue = KEY_Read_All();
  // 屏幕信息显示
  
  
  switch(Now_KeyValue)
  {
  case 1://显示各模块参数
    {
      if(last_KeyValue != 1&&Now_KeyValue ==1)      TFTSPI_CLS(u16WHITE);//防止多次清屏
      
      sprintf((char*)txt,"Roll:%.2f", Roll);         // 显示小车平衡中值
      TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);
      sprintf(txt, "Lamp(%03d,%03d)", sumhang, sumlie);
      TFTSPI_P8X16Str(0, 2, txt, u16RED, u16BLUE);     // 显示红外信标灯在屏幕上的偏差参数
      
      // 电机和编码器参数显示
      sprintf(txt, "M1:%04d, M2:%04d ", MotorDuty2, MotorDuty1);
      TFTSPI_P8X16Str(0, 4, txt, u16RED, u16BLUE);     // 电机1-2数值
      sprintf(txt, "E1:%04d, E2:%04d ", ECPULSE1, ECPULSE2);
      TFTSPI_P8X16Str(0, 5, txt, u16RED, u16BLUE);     // 编码器1-2数值
      sprintf(txt, "BAL_OUT:%.2f", BALANCE_OUT);
      TFTSPI_P8X16Str(0, 6, txt, u16RED, u16BLUE);     // IR灯的均值和期望差值
      sprintf(txt, "Bal:%.2f ",SPEED_OUT);
      TFTSPI_P8X16Str(0, 7, txt, u16RED, u16BLUE);  
            bell_flag=0;

      
    }break;
  case 2:  //按下K1键
    {
      // 显示二值化图像 
      if(last_KeyValue != 2&&Now_KeyValue ==2)      TFTSPI_CLS(u16WHITE);
      TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *) Bin_Image);  
      sprintf(txt, "bal:%.2f", BalDir_out);
      TFTSPI_P6X8Str(0, 15, txt, u16RED, u16BLUE);     // IR灯的均值和期望差值
      sprintf(txt, "error:%.2f", Dir_Error);
      TFTSPI_P8X8Str(10, 15, txt, u16RED, u16BLUE);
            bell_flag=0;

    }break;
  case 3:  //摄像头原图像
    {
      TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
      bell_flag=1;
    }break;
    
  }
}


/*************************************************************************
*  函数名称：void Seek_Beacon (void)
*  功能说明：识别信标灯(简单版,适用于无干扰环境下)
*  参数说明：无
*  函数返回：无
*  修改时间：2022年1月19日
*  备    注：
*************************************************************************/
void Seek_Beacon (void)
{
  uint8_t tm=0;
  uint8_t nr=0; //行
  uint8_t nc=0; //列
  
  dotcnt=0;
  for (nr = 1; nr < LCDH - 1; nr++)
  {
    for (nc = 1; nc < LCDW - 1; nc++)
    {
      // 188*120分辨率下，有两个连续白点，则认为是可能的信标灯
      if ((Bin_Image[nr - 1][nc] + Bin_Image[nr + 1][nc] + Bin_Image[nr][nc + 1] + Bin_Image[nr][nc - 1] > 1))
      {
        dothang[dotcnt]=nr;     // 记录所有的白点水平方向方位，也就是左右偏差
        dotlie[dotcnt++]=nc;    // 记录所有的白点水垂直向方位，也就是远近
      }
    }
  }
  if(dotcnt) // 发现有白斑
  {
    sumlie=0,sumhang=0;         // 清除上次的结果
    for(tm=0;tm<dotcnt;tm++)
    {
      sumlie+=dotlie[tm];     // 所有白点左右偏差值求和
      sumhang+=dothang[tm];   // 所有白点上下偏差值求和，暂时没用到
    }
    sumlie =sumlie/dotcnt;      // 灯的左右中心点
    sumhang=sumhang/dotcnt;     // 灯的远近中心点，暂时没用到
    eye_show();
  }
  return;
}



/*************************************************************************
*  函数名称：void eye_show(void)
*  功能说明：可视化识别出的信标灯的位置，并显示出十字准星
*  参数说明：无
*  函数返回：无
*  修改时间：2022年3月31日
*  备    注：  Bin_Image[LCDH][LCDW]; 二值化后图像数组
*************************************************************************/
void eye_show (void)//显示十字
{
  for(int i = 1 ;i<LCDH-1;i++)
  {
      Bin_Image[i][sumlie] = 1;
  }
  for(int i = 1 ;i<LCDW-1;i++)
  {
      Bin_Image[sumhang][i] = 1;
  }
    return;
}

/*************************************************************************
*  函数名称：float Square_detection (int x,int y)
*  功能说明：对目标进行简单的长宽判断
*  参数说明：无
*  函数返回：无
*  修改时间：2022年7月11日
*  备    注：  Bin_Image[LCDH][LCDW]; 二值化后图像数组
*************************************************************************/
float Square_detection (int x,int y)
{
    uint8_t tm=0;
    uint8_t nr=x; //行
    uint8_t nc=y; //列
    shang=0,xia=0,zuo=0,you=0;  //上下左右
    dotcnt=0;
    
    
    if((Bin_Image[nr][nc] == 1) || (Bin_Image[nr + 1][nc] + Bin_Image[nr + 3][nc]+ Bin_Image[nr - 1][nc] +Bin_Image[nr - 3][nc] +
      Bin_Image[nr][nc + 1] +Bin_Image[nr][nc + 3]+Bin_Image[nr][nc -3] +Bin_Image[nr][nc - 1] >= 4) )
    {
        while (Bin_Image[nr - shang][nc] ==1)
        {
            shang++;
        }
        while (Bin_Image[nr + xia][nc] == 1)
        {
            xia++;
        }
        while (Bin_Image[nr][nc - zuo] == 1)
        {
            zuo++;
        }
        while (Bin_Image[nr][nc + you] == 1)
        {
            you++;
        }
        //求长宽比例        
        return (abs(zuo+you)/2.0) / (abs(shang+xia)/2.0);
        
    }
}



/********************************************
设置权重判断
        面积--高度      亮度      形状
量级  0~1500   0~120   0~255       0~
面积->小  高度->高  可信度->高
面积->大  高度->低  可信度->高

亮度->高->可信度高
形状->高->可信度高
*********************************************/
int quanzhi_weight[100];
void quanzhong_light_panduan(void)
{
    float zz_return;
    memset(quanzhi_weight,0,100);
    for(int i=2;i<=labelcount+2;i++)
    {
        zz_return = Square_detection(Connected_x[i],Connected_y[i]);
        if(zz_return<0.8)//过于竖长
        {
            quanzhi_weight[i]=0;
            continue;
        }
        if(Connected_x[i]<30 && area_map[i]>30)//远处太大
        {
            quanzhi_weight[i]=0;
            continue;
        }
        if(Connected_x[i]>90 && area_map[i]<10)//近处太小
        {
            quanzhi_weight[i]=0;
            continue;
        }
        //权重值越大，是信标灯的概率越大
        //120和1500为当前使用镜头下的参数，不同镜头需要自行修改
        //120为图像上下高度，1500为图像默认情况下可能出现的最大面积
        quanzhi_weight[i]= (int) ((Connected_x[i]*1.0/120)*(1-area_map[i]*1.0/1500)*1000+Connected_light[i]*2+zz_return*100);

    }
    
    
}
//寻找最大权重值
int find_max(void)
{
    int maxx=quanzhi_weight[2];
    int wei=2;
    for(int i=3;i<=labelcount+2;i++)
    {
        if(quanzhi_weight[i]>maxx)
        {
            maxx = quanzhi_weight[i];
            wei=i;
        }
    }
    return wei;
}
//计算最大权重目标位置
void quanzhong_light(void)
{
    dotcnt=0;
    int zxc=find_max();
    
    if(quanzhi_weight[zxc]!=0)
    {
        sumlie = Connected_y[zxc];
        sumhang = Connected_x[zxc];
        dotcnt++;
        
        eye_show();        
    }
}
