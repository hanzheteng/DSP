#include "dm_types.h"
#include "dm_bf5xx.h"
#include <math.h>

/****************************【文件和程序基本情况说明】************************************
* 
* BMP文件：
*   BMP文件是Windows下的标准图片存储格式，本实验中使用的都是以24位真彩色存储的无压缩文件。
*   这些文件被预先处理成.dat文件，便于在实验中使用#include读入。每个图片数据的前54字节是
*   文件头信息，之后是3通道的图片色彩信息。数据顺序是从图片左上角第[0]行第[0]列的像素点
*   开始，以BGR通道顺序，先向右扫描完一整行，再向下扫描各行。即原图第1个像素点对应文件数
*   据的[54][55][56]字节，分别表示第1个像素点的B、G、R通道的数据，数据类型为unsigned char.
* 
* RGB显示屏：
*   显示屏的分辨率为240*320*3，其中3表示RGB三个通道。
*   定义全局变量DisplayBuffer用于屏幕显示，并初始化各个通道值为255，3通道混合后也就是白色。
*   因特殊原因DisplayBuffer的前0-6行不能显示，所以从第7行开始把要显示的图片数据复制进去。
*   所以DisplayBuffer实际数组大小为247*960。
*   BMP文件中通道顺序为BGR，而DisplayBuffer的通道顺序为RGB，如果显示彩色图片要交换RB通道。
*   【注意】DisplayBuffer数组定义不可修改！在工程的其他文件中被extern使用。
* 
* 程序运行流程/输入输出算法流程：
*   由于实验中要处理的图片都是灰度图片，因此在用算法处理图片数据之前，只需要从3通道中提取
*   1通道进行处理，处理后再把1通道通过复制补全成完整的3通道，用RGB来显示就是灰度图像了。
*   最后再把这部分处理过的图片数据复制到DisplayBuffer的左上角替换掉原来的白色255的数据，
*   就能在板子上显示出灰度图。
* 
* 内存分配问题：
*   【1】要使用宏定义+静态内存分配，不建议使用malloc动态分配；
*   ANSI C要求只能使用常量数据类型来静态分配数组空间，因此只能用#define宏定义来简化操作，
*   读取多张图片时也必须进行多张图片的长宽信息宏定义。如果使用malloc动态分配内存不仅不会
*   节省空间，反而还会占用大量片上内存。虽然C语言中对动态分配的内存进行free操作，但实际执
*   行时，由于DSP没有搭载操作系统，无法进行内存管理，所以free语句并没有被执行，所有分配过
*   的内存都无法进行回收。并且malloc语句在每次程序运行中的时间和位置都不确定，会造成系统的
*   不稳定性增加。
*   【2】所有与图片相关的大数组的定义要在主函数外部完成，要预先定义成全局变量；
*   ADSP-BF561型号的板子片上内存只有328KB，在main函数中进行大量内存分配很容易导致板子跑飞，
*   在main函数外部预先定义的全局变量会存储在DSP的外部扩展内存中，因此不占用片上内存。
*
* 其他提醒：
*   【1】对于有多张图片输入输出的情况，可以通过传递全局数组变量的变量名/首地址来封装函数，
*   函数本身类型为void，通过传递数组指针来完成函数的输入输出操作。
*   【2】在main函数中，可以预先定义指针数组来存放各个外部全局数组的地址，也可以预先定义存储
*   长宽信息的数组存放宏定义的长宽数值，方便后面使用for循环进行简化运算。
*   【3】注意宏定义的变量名不要与函数体内定义的变量名重名，否则在编译时函数内变量会被替换成
*   宏定义值。
*   【4】uint8_t uint_t等类型定义包含在dm_types.h头文件内。
*   【5】大多数错误（比如内存溢出板子跑飞）都可以通过重启工程/软件/实验板解决。
*
* Last modified: April 24, 2017
*******************************************************************************************/


/*******************【宏定义图片宽度和高度】*******************************/
//待检测的图片
#define imageW 100
#define imageH 100

//五个脸部模板以及眼睛模板  Width表示宽度  Height表示高度
#define AW 66
#define AH 40

#define BW 69
#define BH 42

#define CW 68
#define CH 40

#define DW 69
#define DH 37

#define EW 68
#define EH 38

#define EYEW 68
#define EYEH 18

//灰度均衡化时要统一的均值和标准差
#define Standard_MEAN 128
#define Standard_SD   50
/***************************************************************************/


/******************************【读入图片数据】***************************/
//待检测的图片
uint8_t Inputdata[imageW*imageH*3+54]={ 
#include "detect.dat"
};

//五个脸部模板及眼睛模板
uint8_t InputA[AW*AH*3+54]={ 
#include "a.dat"
};
uint8_t InputB[BW*BH*3+54]={ 
#include "b.dat"
};
uint8_t InputC[CW*CH*3+54]={ 
#include "c.dat"
};
uint8_t InputD[DW*DH*3+54]={ 
#include "d.dat"
};
uint8_t InputE[EW*EH*3+54]={ 
#include "e.dat"
};
uint8_t InputEYE[EYEW*EYEH*3+54]={ 
#include "eye.dat"
};
/*************************************************************************/


/**************【为图片处理过程中要用到的数组预分配空间】********************/
//图片数据：用于图像处理
uint8_t image[imageW*imageH]; 
uint8_t A[AW*AH];
uint8_t B[BW*BH];
uint8_t C[CW*CH];
uint8_t D[DW*DH];
uint8_t E[EW*EH];
uint8_t EYE[EYEW*EYEH];

//图片Buffer：用于图片数据读入
uint8_t rowImage[imageW*3]; 
uint8_t rowA[AW*3];
uint8_t rowB[BW*3];
uint8_t rowC[CW*3];
uint8_t rowD[DW*3];
uint8_t rowE[EW*3];
uint8_t rowEYE[EYEW*3];

//图片Buffer：用于图片显示（显示哪张图就给哪张图预分配Buffer）
uint8_t displayImage[imageW*imageH*3]; 

//图片Buffer：用于模板匹配时的待匹配区域
uint8_t detA[AW*AH];
uint8_t detB[BW*BH];
uint8_t detC[CW*CH];
uint8_t detD[DW*DH];
uint8_t detE[EW*EH];
uint8_t detEYE[EYEW*EYEH];

//每个像素点进行眼睛模板匹配后的相关程度
float CorrEYE[imageH][imageW];

//要在实验板上进行显示的数据，分辨率240*320*3，其中前0-6行不能显示
uint8_t DisplayBuffer[247][960];  //DisplayBuffer不可修改，在dm_tft_ppiinit.c中被extern
/****************************************************************************/


/**************************************************************************************
* 函数名：TransData
* 功能：将输入数据去掉54字节头文件，去掉多余2通道，只保留单通道unsigned char型灰度矩阵
* 输入参数：inputData  待处理数据的数组首地址
*           imageData  用于存放处理后数据的数组首地址
*           rowBuffer  用于存储图片一行3通道数据的Buffer
*           Width      图片宽度
*           Height     图片高度
* 输出参数：无
**************************************************************************************/
void TransData(uint8_t *inputData, uint8_t *imageData, uint8_t *rowBuffer, uint_t Width, uint_t Height)
{
	unsigned int i,j;
	for(i=0;i<Height;i++)
	{
		//把输入图片每行的所有RGB数据（全部3通道）都存到rowBuffer中
		for(j=0;j<Width*3;j++)
		{
			rowBuffer[j] = inputData[i*Width*3+j+54]; //前54个字节是bmp文件的头数据，跳过	
		}
		
		//只提取rowBuffer中的一个通道的数据（j的3倍）给imageData用作后续处理
		for(j=0;j<Width;j++)
		{
	   		imageData[i*Width+j] = rowBuffer[j*3];
		}
	}
}
/**************************************************************************************/


/***************************************************************************************
* 函数名：DisplayImage
* 功能：将待输出的图片数据复制到DisplayBuffer数组中，用于RGB屏幕显示
* 输入参数：image        待输出的图片数据的数组首地址
*           imageBuffer  用于存储图片3通道数据的Buffer
*           Width        图片宽度
*           Height       图片高度
* 输出参数：无
****************************************************************************************/
void DisplayImage(uint8_t *image, uint8_t *imageBuffer, uint_t Width, uint_t Height)
{
	unsigned int i, j;
	
	//复制相同的单通道数据到3通道，从而使得RGB显示出来是灰度图像
	for(i=0;i<Height*Width;i++)
	{
		imageBuffer[i*3] = image[i];
		imageBuffer[i*3+1] = image[i];
		imageBuffer[i*3+2] = image[i];
	}
	
	//给每个像素点预处理成白色，3通道轮流赋值255
	for(i=0;i<247;i++)
	{	
		for(j=0;j<960;j++)
		{ 	
			DisplayBuffer[i][j]=255;
		}
	}
	
	//从DisplayBuffer的第7行开始写入图像数据，前0-6行被用于其他用途无法显示出来
	for(i=0;i<Height;i++)
	{
		for(j=0;j<Width*3;j++)
		{
			//正常显示
			DisplayBuffer[i+7][j] = imageBuffer[i*Width*3+j];
			
			//倒立显示
			//DisplayBuffer[i+7][j] = imageBuffer[(Height-1-i)*Width*3+j];
		}
	}
}
/***************************************************************************************/


/***********************【函数：求图片数据的均值和标准差/均方差】**************************/
float CalMean(uint8_t *imageData, uint_t length)
{
	unsigned int i;
	float sum=0, mean;
	for(i=0;i<length;i++)
	{
		sum += imageData[i];
	}
	mean = sum / length;
	return mean;
}

float CalSD(uint8_t *imageData, uint_t length, float mean)
{
	unsigned int i;
	float sum=0, SD;
	for(i=0;i<length;i++)
	{
		sum += (imageData[i] - mean)*(imageData[i] - mean);
	}
	SD = sqrt(sum / length);
	return SD;
}
/********************************************************************************************/


/*****************************【函数：灰度均衡化处理】**************************************/
void Equalization(uint8_t *imageData, uint_t length, float m, float SD, float m0, float SD0)
{
	unsigned int i;
	for(i=0;i<length;i++)
	{
		//逐个像素点进行调整，使得总体分布满足目标均值和标准差
		imageData[i] = (imageData[i]-m) * (SD0/SD) + m0;
	}
}
/*******************************************************************************************/


/************************【函数：计算两给定区域的匹配程度】********************************/
float Correlation(uint8_t *Region, uint8_t *Template, uint_t length, float mean, float SD)
{
	unsigned int i;
	float sumR=0, sumD=0, cor, dev, D;
	for(i=0;i<length;i++)
	{
		sumR += (Region[i] - mean) * (Template[i] - mean);
		sumD += (Region[i] - Template[i]) * (Region[i] - Template[i]);
	}
	cor = sumR / (length * SD * SD);   //计算相关系数
	dev = sumD / length;               //计算平均偏差
	D = cor + 35/(1 + dev);  //综合两参数作为匹配程度的度量，取经验系数35
	return D;
}
/*******************************************************************************************/


/********************【函数：计算以某坐标点为顶点的区域匹配程度】****************************/
float PointCorr(uint8_t *Image, uint_t pi, uint_t pj, uint8_t *Region, uint8_t *Template, uint_t W, uint_t H)
{
	unsigned int i, j;
	float Corr;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			//以Image图上的pi,pj点为顶点取出长宽为H,W的区域
			Region[i*W+j] = Image[(pi+i)*W+pj+j];
		}
	}
	//计算取出的Region与待匹配的模板图片Template之间的匹配程度
	Corr = Correlation(Region, Template, W*H, Standard_MEAN, Standard_SD);
	return Corr;
}
/*******************************************************************************************/


/**********************【函数：以某坐标点为顶点对目标进行框定】******************************/
void DrawSquare(uint8_t *Image, uint_t ImageW, uint_t ImageH, uint_t pi, uint_t pj, uint_t W, uint_t H)
{
	unsigned int i, j;
	//画方框左右两边
	for(i=0;i<H+1;i++) 
	{
		Image[(pi+i)*ImageW + pj] = 255;      //左
		Image[(pi+i)*ImageW + pj + W] = 255;  //右
	}
	//画方框上下两边
	for(j=0;j<W+1;j++)
	{
		Image[pi*ImageW + pj + j] = 255;      //上
		Image[(pi+H)*ImageW + pj + j] = 255;  //下
	}
}
/*******************************************************************************************/


/***********************************【主程序main】******************************************/
int main()                                                                                                                                                                               
{	
	Set_PLL( (int16_t)(CORECLK/CLKIN), (int16_t)(CORECLK/SYSCLK));	
	Init_EBIU();
	lcd_init();

	unsigned int i, j, p, q;
	
	//预先定义数组便于使用for循环
	uint8_t *InputPtr[7] = {Inputdata, InputA, InputB, InputC, InputD, InputE, InputEYE};
	uint8_t *ImagePtr[7] = {image, A, B, C, D, E, EYE};
	uint8_t *rowBufferPtr[7] = {rowImage, rowA, rowB, rowC, rowD, rowE, rowEYE};
	uint8_t *detFacePtr[5] = {detA, detB, detC, detD, detE};
	
	uint_t Width[7] = {imageW, AW, BW, CW, DW, EW, EYEW};
	uint_t Height[7] = {imageH, AH, BH, CH, DH, EH, EYEH};
	uint_t Length[7] = {imageW*imageH, AW*AH, BW*BH, CW*CH, DW*DH, EW*EH, EYEW*EYEH};
	
	
	//读取数据，将输入数据转换成8位unsigned char型灰度矩阵
	for(i=0;i<7;i++)
	{
		TransData(InputPtr[i], ImagePtr[i], rowBufferPtr[i], Width[i], Height[i]);
	}
	
	//计算各个图片的均值和标准差/均方差(Standrad Deviation)
	float mean[7]=0, SD[7]=0;
	for(i=0;i<7;i++)
	{
		mean[i] = CalMean(ImagePtr[i], Length[i]);
	}
	for(i=0;i<7;i++)
	{
		SD[i] = CalSD(ImagePtr[i], Length[i], mean[i]);
	}
	
	//灰度均衡化，以待检测图片的灰度分布为标准
	for(i=0;i<7;i++)
	{
		Equalization(ImagePtr[i], Length[i], mean[i], SD[i], Standard_MEAN, Standard_SD);
	}
	
	//首先用眼睛模板进行粗略搜索，记录各点匹配度
	for(i=0;i<(imageH-EYEH);i++)
	{
		for(j=0;j<(imageW-EYEW);j++)
		{
			//每次从待检测图片上取出要进行匹配的图像数据赋给detEYE
			for(p=0;p<EYEH;p++)
			{
				for(q=0;q<EYEW;q++)
				{
					detEYE[p*EYEW+q] = image[(i+p)*imageW+j+q];
				}
			}
			//用眼睛模板计算两区域的匹配度
			CorrEYE[i][j] = Correlation(detEYE, EYE, EYEW*EYEH, Standard_MEAN, Standard_SD);
		}
	}
	
	//找到匹配度最大的三个点，保存坐标值，每个点有i和j两个坐标
	uint_t Point[3][2];  
	float PointMatch[3]=0; 
	for(i=0;i<(imageH-EYEH);i++)
	{
		for(j=0;j<(imageW-EYEW);j++)
		{
			if(CorrEYE[i][j] > PointMatch[0])
			{
				if(CorrEYE[i][j] > PointMatch[1])
				{
					if(CorrEYE[i][j] > PointMatch[2])
					{
						//[0][1][2]匹配度依次增大
						PointMatch[0] = PointMatch[1];
						PointMatch[1] = PointMatch[2];
						PointMatch[2] = CorrEYE[i][j];
						Point[2][0] = i;
						Point[2][1] = j;
					}
					else
					{
						PointMatch[0] = PointMatch[1];
						PointMatch[1] = CorrEYE[i][j];
						Point[1][0] = i;
						Point[1][1] = j;
					}
				}
				else
				{
					PointMatch[0] = CorrEYE[i][j];
					Point[0][0] = i;
					Point[0][1] = j;
				}
			}
		}
	}
	
	//对这三个坐标点，使用五个人脸模板进行匹配，记录最佳匹配点和模板长宽信息
	uint_t objW, objH;
	uint_t BestPoint[2];
	float TempMatch=0, BestMatch=0;

	for(i=0;i<3;i++)
	{
		for(j=0;j<5;j++)
		{
			TempMatch = PointCorr(image, Point[i][0], Point[i][1], detFacePtr[j], ImagePtr[j+1], Width[j+1], Height[j+1]);
			if(TempMatch > BestMatch)
			{
				BestMatch = TempMatch;
				BestPoint[0] = Point[i][0];
				BestPoint[1] = Point[i][1];
				objW = Width[j+1];
				objH = Height[j+1];
			}
		}
	}
	
	//在原图像上把匹配结果区域框出来
	DrawSquare(image, imageW, imageH, BestPoint[0], BestPoint[1], objW, objH);
	
	//显示图片
	DisplayImage(image, displayImage, imageW, imageH);  //只能调用一次，屏幕每次只能显示一张图片
	
	Init_Platform_TFT();  //在dm_tft_ppiinit.c中定义
	while(1){}
}

