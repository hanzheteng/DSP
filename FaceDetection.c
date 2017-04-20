#include "dm_types.h"
#include "dm_bf5xx.h"
#include <math.h>

/************************【文件和程序基本情况】******************************************
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
* 
* 程序运行过程：
*   由于实验中要处理的图片都是灰度图片，因此在用算法处理图片数据之前，只需要从3通道中提取
*   1通道进行处理，处理后再把1通道通过复制补全成完整的3通道，用RGB来显示就是灰度图像了。
*   最后再把这部分处理过的图片数据复制到DisplayBuffer的左上角替换掉原来的白色255的数据，
*   就能在板子上显示出灰度图。
* 
* 注意事项：
*   DisplayBuffer数组定义不可修改！在工程的其他文件中被extern使用。
*   BMP文件中通道顺序为BGR，而DisplayBuffer的通道顺序为RGB，如果显示彩色图片要交换RB通道。
*   考虑到dsp板子的内存限制，在外部预先定义好全局变量运行速度会更快。
*   在函数体内部分配大量空间有一定可能会导致板子无法正常运行。
*   ANSI C不支持const int类型来分配数组，只能用#define宏定义，或者malloc动态分配内存.
*   uint8_t uint_t等类型定义包含在dm_types.h头文件内。
*
*****************************************************************************************/


/*******************【宏定义图片宽度和高度】*******************************/
//待检测的图片
#define detectW 100
#define detectH 100

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
/***************************************************************************/


/******************************【读入图片数据】***************************/
//待检测的图片
uint8_t Inputdata[detectW*detectH*3+54]={ 
#include "detect.dat"
};

//五个脸部模板及眼睛模板
uint8_t Adata[AH*AW*3+54]={ 
#include "a.dat"
};
uint8_t Bdata[BH*BW*3+54]={ 
#include "b.dat"
};
uint8_t Cdata[CH*CW*3+54]={ 
#include "c.dat"
};
uint8_t Ddata[DH*DW*3+54]={ 
#include "d.dat"
};
uint8_t Edata[EH*EW*3+54]={ 
#include "e.dat"
};
uint8_t EYEdata[EYEH*EYEW*3+54]={ 
#include "eye.dat"
};
/*************************************************************************/


/**************【为图片处理过程中要用到的数组预分配空间】********************/
//待检测的图片
uint8_t image[detectH*detectW]; 

//五个脸部模板及眼睛模板
uint8_t A[AH*AW];

uint8_t B[BH*BW];

uint8_t C[CH*CW];

uint8_t D[DH*DW];

uint8_t E[EH*EW];

uint8_t EYE[EYEH*EYEW];

//要在实验板上进行显示的数据，分辨率240*320*3，其中前0-6行不能显示
uint8_t DisplayBuffer[247][960];  //DisplayBuffer不可修改，在dm_tft_ppiinit.c中被extern并用到
/****************************************************************************/


/*************************************************************************************
* 函数名：TransData
* 功能：将输入数据去掉54字节头文件，去掉多余2通道，只保留单通道unsigned char型灰度矩阵
* 输入参数：inputData  待处理数据的数组首地址
*           imageData  用于存放处理后数据的数组首地址
*           Height     图片高度
*           Width      图片宽度
* 输出参数：无
***********************************************************************************/
void TransData(uint8_t *inputData, uint8_t *imageData, uint_t Height, uint_t Width)
{
	unsigned int i,j;
	uint8_t *temp = (uint8_t *)malloc(Width * 3 * sizeof(uint8_t));
	for(i=0;i<Height;i++)
	{
		//把输入图片每行的所有RGB数据（全部3通道）都存到temp中
		for(j=0;j<Width*3;j++)
		{
			temp[j] = inputData[i*Width*3+j+54]; //前54个字节是bmp文件的头数据，跳过	
		}
		
		//只提取temp中的一个通道的数据（j的3倍）给imageData用作后续处理
		for(j=0;j<Width;j++)
		{
	   		imageData[i*Width+j] = temp[j*3];
		}
	}
	free(temp);
}
/*********************************************************************************************/


/*************************************************************************************
* 函数名：DisplayImage
* 功能：将待输出的图片数据复制到DisplayBuffer数组中，用于RGB屏幕显示
* 输入参数：image    待输出的图片数据的数组首地址
*           Height   图片高度
*           Width    图片宽度
* 输出参数：无
***********************************************************************************/
void DisplayImage(uint8_t *image, uint_t Height, uint_t Width)
{
	unsigned int i, j;
	uint8_t *imagebuf = (uint8_t *)malloc(Height * Width * 3 * sizeof(uint8_t));
	
	//复制相同的单通道数据到3通道，从而使得RGB显示出来是灰度图像
	for(i=0;i<Height*Width;i++)
	{
		imagebuf[i*3] = image[i];
		imagebuf[i*3+1] = image[i];
		imagebuf[i*3+2] = image[i];
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
			DisplayBuffer[i+7][j] = imagebuf[i*Width*3+j];
			
			//倒立显示
			//DisplayBuffer[i+7][j] = imagebuf[(Height-1-i)*Width*3+j];
		}
	}
	
	free(imagebuf);
}
/**************************************************************************************/


/**************************【主程序main】************************************************/
int main()                                                                                                                                                                               
{	
	Set_PLL( (int16_t)(CORECLK/CLKIN), (int16_t)(CORECLK/SYSCLK));	
	Init_EBIU();
	lcd_init();

	//读取数据 将输入数据转换成8位unsigned char型灰度矩阵	
	TransData(Inputdata, image, detectH, detectW);
	TransData(Adata, A, AH, AW);
	TransData(Bdata, B, BH, BW);
	TransData(Cdata, C, CH, CW);
	TransData(Ddata, D, DH, DW);
	TransData(Edata, E, EH, EW);
	TransData(EYEdata, EYE, EYEH, EYEW);
	
	/***************************【对图像数据进行处理】********************************/
	
	
	
	
	/*********************************************************************************/
	
	DisplayImage(image, detectH, detectW);
	
	Init_Platform_TFT();  //在dm_tft_ppiinit.c中定义
	while(1){}
}

