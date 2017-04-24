#ifndef __CPU_H__
#define __CPU_H__

#include <ccblkfn.h>
#include <sys/exception.h>
#include "dm_bf5xx.h"
#include "dm_cpu.h"

/****************************************************************************
* 名称 ：Init_SDRAM
* 功能 ：初始化SDRAM
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/
void Init_SDRAM(void)
{
	if (*pEBIU_SDSTAT & SDRS) {
		//SDRAM Refresh Rate Control Register
		*pEBIU_SDRRC = 0x00000817;	
		//SDRAM Memory Bank Control Register
		*pEBIU_SDBCTL = 0x00000013;
		//SDRAM Memory Global Control Register	
		*pEBIU_SDGCTL = 0x0091998d;	
		ssync();
	}														
}

/****************************************************************************
* 名称 ：Init_Flags
* 功能 ：初始化PF口
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/
void Setup_Flags()
{
	*pFIO0_INEN = 0x0001;		 
	*pFIO0_DIR = 0x0000;
	*pFIO2_DIR = 0x0FFF;				// PF32-47 Outputs (LEDs)   
	*pFIO2_INEN	= 0x4000;
	*pFIO2_EDGE	= 0x0000;
	*pFIO2_MASKA_S = 0x4000;
	*pFIO2_POLAR = 0x4000; 
}

/****************************************************************************
* 名称 ：Init_EBIU
* 功能 ：初始化并允许异步BANK存储器工作
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/
void Init_EBIU(void)
{	
	*pEBIU_AMBCTL0	= 0x7bb07bb0;
	*pEBIU_AMBCTL1	= 0xaac0bbd0;
	*pEBIU_AMGCTL	= 0x008F;      //16 bit kongzhi 
}

/**********************************************************************************
* 名称 ：Set_PLL
* 功能 ：初始化内核时钟和系统时钟
* 入口参数 ：CoreCLOCK_multiplier  SCLK_divider 
* 出口参数 ：无
***********************************************************************************/
void Set_PLL(int16_t CoreCLOCK_multiplier, int16_t SCLK_divider)
{ int16_t previous_PLL= *pPLL_CTL;
  int16_t previous_SICA_IWR = *pSICA_IWR0;
  int16_t previous_SICB_IWR = *pSICB_IWR0;
  int16_t new_PLL= (previous_PLL & 0x81ff) | ((CoreCLOCK_multiplier & 0x3f) <<9);
   
  if (new_PLL != previous_PLL) {			  // skip if multiplier has not changed
		  	
	 if ((int_t)(*pSRAM_BASE_ADDRESS) == 0xFF800000 ) {						  // do things for Core A

	 	*pSICB_SYSCR |= 0x0080;				// Raise Core B Supplemental-Int0
		ssync();
		while((*pSICB_SYSCR & 0x080));		// Wait: Core B Acknowledge SUP-B0	
	 	*pSICA_IWR0 = (previous_SICA_IWR | 0x1); // enable PLL Wakeup Interrupt
  		*pPLL_CTL = new_PLL;
		ssync();
  		idle();							  // put in idle
  		*pSICA_IWR0 = previous_SICA_IWR;		  // continue here after idle, restore previous IWR content
		ssync();
	  }
	else{							  		  // do things for Core B
		while(!(*pSICB_SYSCR & 0x0080));		// Wait: For Core A to raise SUP-B0	
		*pSICB_SYSCR = 0x0800;		// Acknowledge Supplemental Interrupt
		ssync();
		*pSICB_IWR0 = (previous_SICB_IWR | 0x1); // enable PLL Wakeup Interrupt
		ssync();
  		idle();							  // put in idle
  		*pSICB_IWR0 = previous_SICB_IWR;		  // continue here after idle, restore previous IWR content
		ssync();
    }
  }  
  *pPLL_DIV = (*pPLL_DIV & 0xFFF0) | SCLK_divider;
  ssync();  
}
#endif
