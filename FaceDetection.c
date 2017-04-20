#include "dm_types.h"
#include "dm_bf5xx.h"
#include <math.h>

/************************���ļ��ͳ�����������******************************************
* 
* BMP�ļ���
*   BMP�ļ���Windows�µı�׼ͼƬ�洢��ʽ����ʵ����ʹ�õĶ�����24λ���ɫ�洢����ѹ���ļ���
*   ��Щ�ļ���Ԥ�ȴ����.dat�ļ���������ʵ����ʹ��#include���롣ÿ��ͼƬ���ݵ�ǰ54�ֽ���
*   �ļ�ͷ��Ϣ��֮����3ͨ����ͼƬɫ����Ϣ������˳���Ǵ�ͼƬ���Ͻǵ�[0]�е�[0]�е����ص�
*   ��ʼ����BGRͨ��˳��������ɨ����һ���У�������ɨ����С���ԭͼ��1�����ص��Ӧ�ļ���
*   �ݵ�[54][55][56]�ֽڣ��ֱ��ʾ��1�����ص��B��G��Rͨ�������ݣ���������Ϊunsigned char.
* 
* RGB��ʾ����
*   ��ʾ���ķֱ���Ϊ240*320*3������3��ʾRGB����ͨ����
*   ����ȫ�ֱ���DisplayBuffer������Ļ��ʾ������ʼ������ͨ��ֵΪ255��3ͨ����Ϻ�Ҳ���ǰ�ɫ��
*   ������ԭ��DisplayBuffer��ǰ0-6�в�����ʾ�����Դӵ�7�п�ʼ��Ҫ��ʾ��ͼƬ���ݸ��ƽ�ȥ��
*   ����DisplayBufferʵ�������СΪ247*960��
* 
* �������й��̣�
*   ����ʵ����Ҫ�����ͼƬ���ǻҶ�ͼƬ����������㷨����ͼƬ����֮ǰ��ֻ��Ҫ��3ͨ������ȡ
*   1ͨ�����д���������ٰ�1ͨ��ͨ�����Ʋ�ȫ��������3ͨ������RGB����ʾ���ǻҶ�ͼ���ˡ�
*   ����ٰ��ⲿ�ִ������ͼƬ���ݸ��Ƶ�DisplayBuffer�����Ͻ��滻��ԭ���İ�ɫ255�����ݣ�
*   �����ڰ�������ʾ���Ҷ�ͼ��
* 
* ע�����
*   DisplayBuffer���鶨�岻���޸ģ��ڹ��̵������ļ��б�externʹ�á�
*   BMP�ļ���ͨ��˳��ΪBGR����DisplayBuffer��ͨ��˳��ΪRGB�������ʾ��ɫͼƬҪ����RBͨ����
*   ���ǵ�dsp���ӵ��ڴ����ƣ����ⲿԤ�ȶ����ȫ�ֱ��������ٶȻ���졣
*   �ں������ڲ���������ռ���һ�����ܻᵼ�°����޷��������С�
*   ANSI C��֧��const int�������������飬ֻ����#define�궨�壬����malloc��̬�����ڴ�.
*   uint8_t uint_t�����Ͷ��������dm_types.hͷ�ļ��ڡ�
*
*****************************************************************************************/


/*******************���궨��ͼƬ��Ⱥ͸߶ȡ�*******************************/
//������ͼƬ
#define detectW 100
#define detectH 100

//�������ģ���Լ��۾�ģ��  Width��ʾ���  Height��ʾ�߶�
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


/******************************������ͼƬ���ݡ�***************************/
//������ͼƬ
uint8_t Inputdata[detectW*detectH*3+54]={ 
#include "detect.dat"
};

//�������ģ�弰�۾�ģ��
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


/**************��ΪͼƬ���������Ҫ�õ�������Ԥ����ռ䡿********************/
//������ͼƬ
uint8_t image[detectH*detectW]; 

//�������ģ�弰�۾�ģ��
uint8_t A[AH*AW];

uint8_t B[BH*BW];

uint8_t C[CH*CW];

uint8_t D[DH*DW];

uint8_t E[EH*EW];

uint8_t EYE[EYEH*EYEW];

//Ҫ��ʵ����Ͻ�����ʾ�����ݣ��ֱ���240*320*3������ǰ0-6�в�����ʾ
uint8_t DisplayBuffer[247][960];  //DisplayBuffer�����޸ģ���dm_tft_ppiinit.c�б�extern���õ�
/****************************************************************************/


/*************************************************************************************
* ��������TransData
* ���ܣ�����������ȥ��54�ֽ�ͷ�ļ���ȥ������2ͨ����ֻ������ͨ��unsigned char�ͻҶȾ���
* ���������inputData  ���������ݵ������׵�ַ
*           imageData  ���ڴ�Ŵ�������ݵ������׵�ַ
*           Height     ͼƬ�߶�
*           Width      ͼƬ���
* �����������
***********************************************************************************/
void TransData(uint8_t *inputData, uint8_t *imageData, uint_t Height, uint_t Width)
{
	unsigned int i,j;
	uint8_t *temp = (uint8_t *)malloc(Width * 3 * sizeof(uint8_t));
	for(i=0;i<Height;i++)
	{
		//������ͼƬÿ�е�����RGB���ݣ�ȫ��3ͨ�������浽temp��
		for(j=0;j<Width*3;j++)
		{
			temp[j] = inputData[i*Width*3+j+54]; //ǰ54���ֽ���bmp�ļ���ͷ���ݣ�����	
		}
		
		//ֻ��ȡtemp�е�һ��ͨ�������ݣ�j��3������imageData������������
		for(j=0;j<Width;j++)
		{
	   		imageData[i*Width+j] = temp[j*3];
		}
	}
	free(temp);
}
/*********************************************************************************************/


/*************************************************************************************
* ��������DisplayImage
* ���ܣ����������ͼƬ���ݸ��Ƶ�DisplayBuffer�����У�����RGB��Ļ��ʾ
* ���������image    �������ͼƬ���ݵ������׵�ַ
*           Height   ͼƬ�߶�
*           Width    ͼƬ���
* �����������
***********************************************************************************/
void DisplayImage(uint8_t *image, uint_t Height, uint_t Width)
{
	unsigned int i, j;
	uint8_t *imagebuf = (uint8_t *)malloc(Height * Width * 3 * sizeof(uint8_t));
	
	//������ͬ�ĵ�ͨ�����ݵ�3ͨ�����Ӷ�ʹ��RGB��ʾ�����ǻҶ�ͼ��
	for(i=0;i<Height*Width;i++)
	{
		imagebuf[i*3] = image[i];
		imagebuf[i*3+1] = image[i];
		imagebuf[i*3+2] = image[i];
	}
	
	//��ÿ�����ص�Ԥ����ɰ�ɫ��3ͨ��������ֵ255
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
			DisplayBuffer[i+7][j] = imagebuf[i*Width*3+j];
			
			//������ʾ
			//DisplayBuffer[i+7][j] = imagebuf[(Height-1-i)*Width*3+j];
		}
	}
	
	free(imagebuf);
}
/**************************************************************************************/


/**************************��������main��************************************************/
int main()                                                                                                                                                                               
{	
	Set_PLL( (int16_t)(CORECLK/CLKIN), (int16_t)(CORECLK/SYSCLK));	
	Init_EBIU();
	lcd_init();

	//��ȡ���� ����������ת����8λunsigned char�ͻҶȾ���	
	TransData(Inputdata, image, detectH, detectW);
	TransData(Adata, A, AH, AW);
	TransData(Bdata, B, BH, BW);
	TransData(Cdata, C, CH, CW);
	TransData(Ddata, D, DH, DW);
	TransData(Edata, E, EH, EW);
	TransData(EYEdata, EYE, EYEH, EYEW);
	
	/***************************����ͼ�����ݽ��д���********************************/
	
	
	
	
	/*********************************************************************************/
	
	DisplayImage(image, detectH, detectW);
	
	Init_Platform_TFT();  //��dm_tft_ppiinit.c�ж���
	while(1){}
}

