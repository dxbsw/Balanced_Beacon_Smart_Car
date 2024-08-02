
#ifndef __LQ_CAMERA_H
#define __LQ_CAMERA_H

#include "LQ_MT9V034.h"
#include "LQ_AAConfig.h"


#define IMAGEH  MT9V034_IMAGEH   /*!< 摄像头采集高度 */
#define IMAGEW  MT9V034_IMAGEW   /*!< 摄像头采集宽度 */

/* 使用数组宽高 修改这里即可 */
#define LCDH    120  /*!< TFT显示高度（用户使用）高度 */
#define LCDW    160  /*!< TFT显示宽度（用户使用）宽度 */


//#define SMALLIMAGE  // 使用小尺寸显示60*94
#define MAX_ROW   LCDH
#define MAX_COL   LCDW

extern int16_t OFFSET0;      //最远处，赛道中心值综合偏移量
extern int16_t OFFSET1;      //第二格
extern int16_t OFFSET2;      //最近，第三格

/** 图像原始数据存放 */
extern unsigned char Image_Data[IMAGEH][IMAGEW];

/** 压缩后之后用于存放屏幕显示数据  */
extern unsigned char Image_Use[LCDH][LCDW];

/** 二值化后用于OLED显示的数据 */
extern unsigned char Bin_Image[LCDH][LCDW];



void MT9V034_DMA_START(void);
void MT9V034_DMA_CLS(void);
void CAMERA_Reprot(void);
void CAMERA_Init(unsigned char fps);
void Get_Use_Image(void);
void Get_Bin_Image(unsigned char mode);

/*!
* @brief    大津法求阈值大小 
*
* @param    tmImage ： 图像数据
*
* @return   阈值
*
* @note     参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
* @note     https://www.cnblogs.com/moon1992/p/5092726.html
* @note     https://www.cnblogs.com/zhonghuasong/p/7250540.html     
* @note     Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
* @note     1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
* @note     2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
* @note     3) i表示分类的阈值，也即一个灰度级，从0开始迭代	1
* @note     4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
* @note     5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
* @note     6) i++；转到4)，直到i为256时结束迭代
* @note     7) 将最大g相应的i值作为图像的全局阈值
* @note     缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
* @note     解决光照不均匀  https://blog.csdn.net/kk55guang2/article/details/78475414
* @note     https://blog.csdn.net/kk55guang2/article/details/78490069
* @note     https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
*
* @see      GetOSTU(Image_Use);//大津法阈值
*
* @date     2019/6/25 星期二
*/ 
short GetOSTU(unsigned char tmImage[LCDH][LCDW]);


/*!
* @brief    摄像头测试例程
*
* @param
*
* @return
*
* @note     测试MT9V034  注意需要使用  带与非门版（白色）转接座
*
* @example
*
* @date     2019/10/22 星期二
*/
void Test_CAMERA(void);


/*!
* @brief    基于soble边沿检测算子的一种边沿检测
*
* @param    imageIn    输入数组
*           imageOut   输出数组      保存的二值化后的边沿信息
*           Threshold  阈值
*
* @return
*
* @note
*
* @example
*
* @date     2020/5/15
*/
void lq_sobel(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold);


/*!
* @brief    基于soble边沿检测算子的一种自动阈值边沿检测
*
* @param    imageIn    输入数组
*           imageOut   输出数组      保存的二值化后的边沿信息
*
* @return
*
* @note
*
* @example
*
* @date     2020/5/15
*/
void lq_sobelAutoThreshold(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW]);
void Seek_Road(void);
void Bin_Image_Filter(void);



  extern uint8_t labelBuffer[LCDW*LCDH];//标记矩阵
  extern int area_map[100]; //面积矩阵
  
  extern int Connected_x[100];
  extern int Connected_y[100];
  extern int32_t Connected_light[100];//存每个区域的平均亮度
  
//定义队列,队列中两端都会变化,所以用头尾指针表示两端的变化
typedef struct Queue //队列属性
{
 struct QNode* first;
 struct QNode* last;
}Queue;//队列特别记下队首和队尾

typedef struct QNode
{//队列节点属性
 int data;//当前的值
 struct QNode *next;//指向下一个的值
}QNode;


void ConnectedComponentLabeling(unsigned char *bitmap, unsigned char * labelmap);
void SearchNeighbor_4(unsigned char *bitmap, unsigned char *labelmap,uint16_t labelIndex,uint16_t pixelIndex, Queue *queue,int x,int y,unsigned char *lightmap);

int PopQueue(Queue *queue);
void PushQueue(Queue *queue,int data);






#endif



