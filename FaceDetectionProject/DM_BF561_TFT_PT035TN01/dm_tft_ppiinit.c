#include "dm_bf5xx.h"
#include "dm_types.h"

/****************************************************************************
* 功能 ： 外部声明的变量
****************************************************************************/
extern uint8_t DisplayBuffer[][];

/****************************************************************************
* 名称 ： Init_PPI
* 功能 ： PPI 控制器初始化
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/
static void Init_TFTPPI(void)
{	
	*pPPI0_DELAY = 0xca;		//106 cycle delay before writing out data
	*pPPI0_COUNT = 959;		//960 samples per line
}
/****************************************************************************
* 名称 ： Init_PPIDMA
* 功能 ： PPIDMA 初始化
* 入口参数 ：无
* 出口参数 ：无
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
* 名称 ： Init_Timer
* 功能 ： 初始化时钟为PWM 模式  TMR1 和 TMR2 输出
* 入口参数 ：无
* 出口参数 ：无
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
	*pCtrOut_Flag_OE = ~TFT_245_OE ; //CMOS_OE 245 允许 ADG_OE=1
	mdelay(500);
    *pCtrOut_Flag_B  = ~TFTCLK_SET;
    mdelay(500);
}


/****************************************************************************
* 名称 ： PPI_TMR_DMA_Enable
* 功能 ：  PPI 控制器 DMA 控制器 和TMR 控制器 允许 
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/
static void PPI_TMR_DMA_Enable(void)
{
	*pDMA1_0_CONFIG |= 0x1;
	asm("ssync;");
	*pPPI0_CONTROL =  0x809f;
	Init_Timer();			
	*pPPI0_CONTROL |= 0x1; 		//使能PPI
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

