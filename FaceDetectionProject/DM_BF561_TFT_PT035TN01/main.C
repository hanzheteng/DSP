#include "dm_types.h"
#include "dm_bf5xx.h"
#include <math.h>

/****************************���ļ��ͳ���������˵����************************************
* 
* BMP�ļ���
*   BMP�ļ���Windows�µı�׼ͼƬ�洢��ʽ����ʵ����ʹ�õĶ�����24λ���ɫ�洢����ѹ���ļ���
*   ��Щ�ļ���Ԥ�ȴ�����.dat�ļ���������ʵ����ʹ��#include���롣ÿ��ͼƬ���ݵ�ǰ54�ֽ���
*   �ļ�ͷ��Ϣ��֮����3ͨ����ͼƬɫ����Ϣ������˳���Ǵ�ͼƬ���Ͻǵ�[0]�е�[0]�е����ص�
*   ��ʼ����BGRͨ��˳��������ɨ����һ���У�������ɨ����С���ԭͼ��1�����ص��Ӧ�ļ���
*   �ݵ�[54][55][56]�ֽڣ��ֱ��ʾ��1�����ص��B��G��Rͨ�������ݣ���������Ϊunsigned char.
* 
* RGB��ʾ����
*   ��ʾ���ķֱ���Ϊ240*320*3������3��ʾRGB����ͨ����
*   ����ȫ�ֱ���DisplayBuffer������Ļ��ʾ������ʼ������ͨ��ֵΪ255��3ͨ����Ϻ�Ҳ���ǰ�ɫ��
*   ������ԭ��DisplayBuffer��ǰ0-6�в�����ʾ�����Դӵ�7�п�ʼ��Ҫ��ʾ��ͼƬ���ݸ��ƽ�ȥ��
*   ����DisplayBufferʵ�������СΪ247*960��
*   BMP�ļ���ͨ��˳��ΪBGR����DisplayBuffer��ͨ��˳��ΪRGB�������ʾ��ɫͼƬҪ����RBͨ����
*   ��ע�⡿DisplayBuffer���鶨�岻���޸ģ��ڹ��̵������ļ��б�externʹ�á�
* 
* ������������/��������㷨���̣�
*   ����ʵ����Ҫ������ͼƬ���ǻҶ�ͼƬ����������㷨����ͼƬ����֮ǰ��ֻ��Ҫ��3ͨ������ȡ
*   1ͨ�����д������������ٰ�1ͨ��ͨ�����Ʋ�ȫ��������3ͨ������RGB����ʾ���ǻҶ�ͼ���ˡ�
*   ����ٰ��ⲿ�ִ�������ͼƬ���ݸ��Ƶ�DisplayBuffer�����Ͻ��滻��ԭ���İ�ɫ255�����ݣ�
*   �����ڰ�������ʾ���Ҷ�ͼ��
* 
* �ڴ�������⣺
*   ��1��Ҫʹ�ú궨��+��̬�ڴ���䣬������ʹ��malloc��̬���䣻
*   ANSI CҪ��ֻ��ʹ�ó���������������̬��������ռ䣬���ֻ����#define�궨�����򻯲�����
*   ��ȡ����ͼƬʱҲ������ж���ͼƬ�ĳ�����Ϣ�궨�塣���ʹ��malloc��̬�����ڴ治������
*   ��ʡ�ռ䣬��������ռ�ô���Ƭ���ڴ档��ȻC�����жԶ�̬������ڴ����free��������ʵ��ִ
*   ��ʱ������DSPû�д��ز���ϵͳ���޷������ڴ����������free��䲢û�б�ִ�У����з����
*   ���ڴ涼�޷����л��ա�����malloc�����ÿ�γ��������е�ʱ���λ�ö���ȷ���������ϵͳ��
*   ���ȶ������ӡ�
*   ��2��������ͼƬ��صĴ�����Ķ���Ҫ���������ⲿ��ɣ�ҪԤ�ȶ����ȫ�ֱ�����
*   ADSP-BF561�ͺŵİ���Ƭ���ڴ�ֻ��328KB����main�����н��д����ڴ��������׵��°����ܷɣ�
*   ��main�����ⲿԤ�ȶ����ȫ�ֱ�����洢��DSP���ⲿ��չ�ڴ��У���˲�ռ��Ƭ���ڴ档
*
* �������ѣ�
*   ��1�������ж���ͼƬ������������������ͨ������ȫ����������ı�����/�׵�ַ����װ������
*   ������������Ϊvoid��ͨ����������ָ������ɺ������������������
*   ��2����main�����У�����Ԥ�ȶ���ָ����������Ÿ����ⲿȫ������ĵ�ַ��Ҳ����Ԥ�ȶ���洢
*   ������Ϣ�������ź궨��ĳ�����ֵ���������ʹ��forѭ�����м����㡣
*   ��3��ע��궨��ı�������Ҫ�뺯�����ڶ���ı����������������ڱ���ʱ�����ڱ����ᱻ�滻��
*   �궨��ֵ��
*   ��4��uint8_t uint_t�����Ͷ��������dm_types.hͷ�ļ��ڡ�
*   ��5����������󣨱����ڴ���������ܷɣ�������ͨ����������/����/ʵ�������
*
* Last modified: April 24, 2017
*******************************************************************************************/


/*******************���궨��ͼƬ���Ⱥ͸߶ȡ�*******************************/
//������ͼƬ
#define imageW 100
#define imageH 100

//�������ģ���Լ��۾�ģ��  Width��ʾ����  Height��ʾ�߶�
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

//�ҶȾ��⻯ʱҪͳһ�ľ�ֵ�ͱ�׼��
#define Standard_MEAN 128
#define Standard_SD   50
/***************************************************************************/


/******************************������ͼƬ���ݡ�***************************/
//������ͼƬ
uint8_t Inputdata[imageW*imageH*3+54]={ 
#include "detect.dat"
};

//�������ģ�弰�۾�ģ��
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


/**************��ΪͼƬ����������Ҫ�õ�������Ԥ����ռ䡿********************/
//ͼƬ���ݣ�����ͼ����
uint8_t image[imageW*imageH]; 
uint8_t A[AW*AH];
uint8_t B[BW*BH];
uint8_t C[CW*CH];
uint8_t D[DW*DH];
uint8_t E[EW*EH];
uint8_t EYE[EYEW*EYEH];

//ͼƬBuffer������ͼƬ���ݶ���
uint8_t rowImage[imageW*3]; 
uint8_t rowA[AW*3];
uint8_t rowB[BW*3];
uint8_t rowC[CW*3];
uint8_t rowD[DW*3];
uint8_t rowE[EW*3];
uint8_t rowEYE[EYEW*3];

//ͼƬBuffer������ͼƬ��ʾ����ʾ����ͼ�͸�����ͼԤ����Buffer��
uint8_t displayImage[imageW*imageH*3]; 

//ͼƬBuffer������ģ��ƥ��ʱ�Ĵ�ƥ������
uint8_t detA[AW*AH];
uint8_t detB[BW*BH];
uint8_t detC[CW*CH];
uint8_t detD[DW*DH];
uint8_t detE[EW*EH];
uint8_t detEYE[EYEW*EYEH];

//ÿ�����ص�����۾�ģ��ƥ������س̶�
float CorrEYE[imageH][imageW];

//Ҫ��ʵ����Ͻ�����ʾ�����ݣ��ֱ���240*320*3������ǰ0-6�в�����ʾ
uint8_t DisplayBuffer[247][960];  //DisplayBuffer�����޸ģ���dm_tft_ppiinit.c�б�extern
/****************************************************************************/


/**************************************************************************************
* ��������TransData
* ���ܣ�����������ȥ��54�ֽ�ͷ�ļ���ȥ������2ͨ����ֻ������ͨ��unsigned char�ͻҶȾ���
* ���������inputData  ���������ݵ������׵�ַ
*           imageData  ���ڴ�Ŵ��������ݵ������׵�ַ
*           rowBuffer  ���ڴ洢ͼƬһ��3ͨ�����ݵ�Buffer
*           Width      ͼƬ����
*           Height     ͼƬ�߶�
* �����������
**************************************************************************************/
void TransData(uint8_t *inputData, uint8_t *imageData, uint8_t *rowBuffer, uint_t Width, uint_t Height)
{
	unsigned int i,j;
	for(i=0;i<Height;i++)
	{
		//������ͼƬÿ�е�����RGB���ݣ�ȫ��3ͨ�������浽rowBuffer��
		for(j=0;j<Width*3;j++)
		{
			rowBuffer[j] = inputData[i*Width*3+j+54]; //ǰ54���ֽ���bmp�ļ���ͷ���ݣ�����	
		}
		
		//ֻ��ȡrowBuffer�е�һ��ͨ�������ݣ�j��3������imageData������������
		for(j=0;j<Width;j++)
		{
	   		imageData[i*Width+j] = rowBuffer[j*3];
		}
	}
}
/**************************************************************************************/


/***************************************************************************************
* ��������DisplayImage
* ���ܣ����������ͼƬ���ݸ��Ƶ�DisplayBuffer�����У�����RGB��Ļ��ʾ
* ���������image        �������ͼƬ���ݵ������׵�ַ
*           imageBuffer  ���ڴ洢ͼƬ3ͨ�����ݵ�Buffer
*           Width        ͼƬ����
*           Height       ͼƬ�߶�
* �����������
****************************************************************************************/
void DisplayImage(uint8_t *image, uint8_t *imageBuffer, uint_t Width, uint_t Height)
{
	unsigned int i, j;
	
	//������ͬ�ĵ�ͨ�����ݵ�3ͨ�����Ӷ�ʹ��RGB��ʾ�����ǻҶ�ͼ��
	for(i=0;i<Height*Width;i++)
	{
		imageBuffer[i*3] = image[i];
		imageBuffer[i*3+1] = image[i];
		imageBuffer[i*3+2] = image[i];
	}
	
	//��ÿ�����ص�Ԥ�����ɰ�ɫ��3ͨ��������ֵ255
	for(i=0;i<247;i++)
	{	
		for(j=0;j<960;j++)
		{ 	
			DisplayBuffer[i][j]=255;
		}
	}
	
	//��DisplayBuffer�ĵ�7�п�ʼд��ͼ�����ݣ�ǰ0-6�б�����������;�޷���ʾ����
	for(i=0;i<Height;i++)
	{
		for(j=0;j<Width*3;j++)
		{
			//������ʾ
			DisplayBuffer[i+7][j] = imageBuffer[i*Width*3+j];
			
			//������ʾ
			//DisplayBuffer[i+7][j] = imageBuffer[(Height-1-i)*Width*3+j];
		}
	}
}
/***************************************************************************************/


/***********************����������ͼƬ���ݵľ�ֵ�ͱ�׼��/�����**************************/
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


/*****************************���������ҶȾ��⻯������**************************************/
void Equalization(uint8_t *imageData, uint_t length, float m, float SD, float m0, float SD0)
{
	unsigned int i;
	for(i=0;i<length;i++)
	{
		//������ص���е�����ʹ������ֲ�����Ŀ���ֵ�ͱ�׼��
		imageData[i] = (imageData[i]-m) * (SD0/SD) + m0;
	}
}
/*******************************************************************************************/


/************************�����������������������ƥ��̶ȡ�********************************/
float Correlation(uint8_t *Region, uint8_t *Template, uint_t length, float mean, float SD)
{
	unsigned int i;
	float sumR=0, sumD=0, cor, dev, D;
	for(i=0;i<length;i++)
	{
		sumR += (Region[i] - mean) * (Template[i] - mean);
		sumD += (Region[i] - Template[i]) * (Region[i] - Template[i]);
	}
	cor = sumR / (length * SD * SD);   //�������ϵ��
	dev = sumD / length;               //����ƽ��ƫ��
	D = cor + 35/(1 + dev);  //�ۺ���������Ϊƥ��̶ȵĶ�����ȡ����ϵ��35
	return D;
}
/*******************************************************************************************/


/********************��������������ĳ�����Ϊ���������ƥ��̶ȡ�****************************/
float PointCorr(uint8_t *Image, uint_t pi, uint_t pj, uint8_t *Region, uint8_t *Template, uint_t W, uint_t H)
{
	unsigned int i, j;
	float Corr;
	for(i=0;i<H;i++)
	{
		for(j=0;j<W;j++)
		{
			//��Imageͼ�ϵ�pi,pj��Ϊ����ȡ������ΪH,W������
			Region[i*W+j] = Image[(pi+i)*W+pj+j];
		}
	}
	//����ȡ����Region���ƥ���ģ��ͼƬTemplate֮���ƥ��̶�
	Corr = Correlation(Region, Template, W*H, Standard_MEAN, Standard_SD);
	return Corr;
}
/*******************************************************************************************/


/**********************����������ĳ�����Ϊ�����Ŀ����п򶨡�******************************/
void DrawSquare(uint8_t *Image, uint_t ImageW, uint_t ImageH, uint_t pi, uint_t pj, uint_t W, uint_t H)
{
	unsigned int i, j;
	//��������������
	for(i=0;i<H+1;i++) 
	{
		Image[(pi+i)*ImageW + pj] = 255;      //��
		Image[(pi+i)*ImageW + pj + W] = 255;  //��
	}
	//��������������
	for(j=0;j<W+1;j++)
	{
		Image[pi*ImageW + pj + j] = 255;      //��
		Image[(pi+H)*ImageW + pj + j] = 255;  //��
	}
}
/*******************************************************************************************/


/***********************************��������main��******************************************/
int main()                                                                                                                                                                               
{	
	Set_PLL( (int16_t)(CORECLK/CLKIN), (int16_t)(CORECLK/SYSCLK));	
	Init_EBIU();
	lcd_init();

	unsigned int i, j, p, q;
	
	//Ԥ�ȶ����������ʹ��forѭ��
	uint8_t *InputPtr[7] = {Inputdata, InputA, InputB, InputC, InputD, InputE, InputEYE};
	uint8_t *ImagePtr[7] = {image, A, B, C, D, E, EYE};
	uint8_t *rowBufferPtr[7] = {rowImage, rowA, rowB, rowC, rowD, rowE, rowEYE};
	uint8_t *detFacePtr[5] = {detA, detB, detC, detD, detE};
	
	uint_t Width[7] = {imageW, AW, BW, CW, DW, EW, EYEW};
	uint_t Height[7] = {imageH, AH, BH, CH, DH, EH, EYEH};
	uint_t Length[7] = {imageW*imageH, AW*AH, BW*BH, CW*CH, DW*DH, EW*EH, EYEW*EYEH};
	
	
	//��ȡ���ݣ�����������ת����8λunsigned char�ͻҶȾ���
	for(i=0;i<7;i++)
	{
		TransData(InputPtr[i], ImagePtr[i], rowBufferPtr[i], Width[i], Height[i]);
	}
	
	//�������ͼƬ�ľ�ֵ�ͱ�׼��/������(Standrad Deviation)
	float mean[7]=0, SD[7]=0;
	for(i=0;i<7;i++)
	{
		mean[i] = CalMean(ImagePtr[i], Length[i]);
	}
	for(i=0;i<7;i++)
	{
		SD[i] = CalSD(ImagePtr[i], Length[i], mean[i]);
	}
	
	//�ҶȾ��⻯���Դ����ͼƬ�ĻҶȷֲ�Ϊ��׼
	for(i=0;i<7;i++)
	{
		Equalization(ImagePtr[i], Length[i], mean[i], SD[i], Standard_MEAN, Standard_SD);
	}
	
	//�������۾�ģ����д�����������¼����ƥ���
	for(i=0;i<(imageH-EYEH);i++)
	{
		for(j=0;j<(imageW-EYEW);j++)
		{
			//ÿ�δӴ����ͼƬ��ȡ��Ҫ����ƥ���ͼ�����ݸ���detEYE
			for(p=0;p<EYEH;p++)
			{
				for(q=0;q<EYEW;q++)
				{
					detEYE[p*EYEW+q] = image[(i+p)*imageW+j+q];
				}
			}
			//���۾�ģ������������ƥ���
			CorrEYE[i][j] = Correlation(detEYE, EYE, EYEW*EYEH, Standard_MEAN, Standard_SD);
		}
	}
	
	//�ҵ�ƥ������������㣬��������ֵ��ÿ������i��j��������
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
						//[0][1][2]ƥ�����������
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
	
	//������������㣬ʹ���������ģ�����ƥ�䣬��¼���ƥ����ģ�峤����Ϣ
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
	
	//��ԭͼ���ϰ�ƥ������������
	DrawSquare(image, imageW, imageH, BestPoint[0], BestPoint[1], objW, objH);
	
	//��ʾͼƬ
	DisplayImage(image, displayImage, imageW, imageH);  //ֻ�ܵ���һ�Σ���Ļÿ��ֻ����ʾһ��ͼƬ
	
	Init_Platform_TFT();  //��dm_tft_ppiinit.c�ж���
	while(1){}
}
