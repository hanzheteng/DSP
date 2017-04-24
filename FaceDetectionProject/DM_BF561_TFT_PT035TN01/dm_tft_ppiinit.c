#include "dm_bf5xx.h"
#include "dm_types.h"

/****************************************************************************
* ���� �� �ⲿ�����ı���
****************************************************************************/
extern uint8_t DisplayBuffer[][];

/****************************************************************************
* ���� �� Init_PPI
* ���� �� PPI ��������ʼ��
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/
static void Init_TFTPPI(void)
{	
	*pPPI0_DELAY = 0xca;		//106 cycle delay before writing out data
	*pPPI0_COUNT = 959;		//960 samples per line
}
/****************************************************************************
* ���� �� Init_PPIDMA
* ���� �� PPIDMA ��ʼ��
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/
static void Init_PPIDMA(void)
{
	*pDMA1_0_START_ADDR = &DisplayBuffer;
	*pDMA1_0_X_COUNT = 960/2;
	*pDMA1_0_X_MODIFY = 2;
	*pDMA1_0_Y_COUNT = 262;
	*pDMA1_0_Y_MODIFY = 2;
	*pDMA1_0_CONFIG = 0x1034;
}
/****************************************************************************
* ���� �� Init_Timer
* ���� �� ��ʼ��ʱ��ΪPWM ģʽ  TMR1 �� TMR2 ���
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/
static void Init_Timer(void)
{
	*pTIMER8_PERIOD	= 1224;
	*pTIMER8_WIDTH	= 88;
	*pTIMER8_CONFIG	= 0x00a9;
	*pTIMER9_PERIOD	= 320688;
	*pTIMER9_WIDTH	= 3672;
	*pTIMER9_CONFIG	= 0x00a9;
}
  //v5 
static void Init_TFTCPLD(void)  
{
	*pCtrOut_Flag_OE = ~TFT_245_OE ; //CMOS_OE 245 ���� ADG_OE=1
	mdelay(500);
    *pCtrOut_Flag_B  = ~TFTCLK_SET;
    mdelay(500);
}


/****************************************************************************
* ���� �� PPI_TMR_DMA_Enable
* ���� ��  PPI ������ DMA ������ ��TMR ������ ���� 
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/
static void PPI_TMR_DMA_Enable(void)
{
	*pDMA1_0_CONFIG |= 0x1;
	asm("ssync;");
	*pPPI0_CONTROL =  0x809f;
	Init_Timer();			
	*pPPI0_CONTROL |= 0x1; 		//ʹ��PPI
	asm("ssync;");
	*pTMRS4_ENABLE = 0x0003;
	asm("ssync;");
}

void Init_Platform_TFT(void)
{
//	Init_EBIU();
    Init_TFTCPLD();
	Init_TFTPPI();
	Init_PPIDMA();
	PPI_TMR_DMA_Enable();
}

void PPI_TMR_DMAR_Disable(void)
{
	*pDMA1_0_CONFIG &= (~DMA_EN);
	*pPPI0_CONTROL &= (~PORT_EN);
	*pTMRS4_DISABLE |= (TMR8_EN|TMR9_EN);
}

