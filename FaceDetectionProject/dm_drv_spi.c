#include <cdefBF561.h>
#include "dm_bf5xx.h"

void Drv_Config_slave_read()
{	
	*pSPI_CTL = (((*pSPI_CTL)&0xFFFC)|TIMOD_R);
	asm("ssync;");
}

void Drv_Config_slave_read_with_DMA()
{
	*pSPI_CTL = (((*pSPI_CTL)&0xFFFC)|TIMOD_RDMA);
	asm("ssync;");
}

void Drv_config_slave_write()
{
	*pSPI_CTL |= (((*pSPI_CTL)&0xFFFC)|TIMOD_T)|EMISO;
	asm("ssync;");
}

void Drv_Config_master_read()
{
	*pSPI_CTL = (((*pSPI_CTL)&0xFFFC)|TIMOD_R)|EMISO;
	asm("ssync;");
}

void Drv_config_master_write()
{
	*pSPI_CTL |= (((*pSPI_CTL)&0xFFFC)|TIMOD_T);
	asm("ssync;");
}

void Drv_config_to_master()
{
	*pSPI_FLG = FLS2;		// Set baud rate SPI_SCK = HCLK/(2*SPIBAUD) SCK
	asm("ssync;");
	*pSPI_BAUD = 6000;		//SPI_CLK = 30MHz
	asm("ssync;");
	*pSPI_CTL = MSTR;		//主模式
	asm("ssync;");
}


void Drv_config_to_slave()
{
	*pSPI_CTL = PSSE;
	asm("ssync;");
}

void Drv_SPI_enable()
{
	*pSPI_CTL |= SPE;
	asm("ssync;");	
}

void Drv_SPI_disable()
{
	*pSPI_CTL &= ~SPE;
	asm("ssync;");
}

void Drv_clean_revbuf(void)
{
	int_t temp;   	
    temp = *pSPI_RDBR;
    asm("ssync;");
}

void SPI_tdbr(uint8_t addr)
{
	*pSPI_TDBR = addr;
	asm("ssync;");
	while(*pSPI_STAT & TXS);
}

uint8_t SPI_rdbr()
{
	while(!(*pSPI_STAT & RXS));
	return *pSPI_RDBR;
}

void SPIenable()
{
	*pSPI_CTL=*pSPI_CTL|SPI_EN;
	asm("ssync;");
}

void SPIdisable()
{
	*pSPI_CTL &=~SPI_EN;
	asm("ssync;");
}

void SPIsend(uint8_t p)
{
	*pSPI_TDBR = p;
	asm("ssync;");
	while(*pSPI_STAT&TXS);
}

uint8_t SPIReadByte(void)
{	
	*pSPI_RDBR = 0xff;
	asm("ssync;");
	while(!(*pSPI_STAT&RXS));	
	return (*pSPI_RDBR);
}

/****************************************************************************
* 名称 ：SPIinit
* 功能 ：初始化SPI控制器 波特率为1M 数据8位 先输出MSB
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/ 

void SPIinit()
{	
	*pSPI_BAUD=30; 
	*pSPI_FLG=0x0004;  
	*pSPI_CTL = 0x0400;
	*pSPI_CTL|=MSTR|CPOL|EMISO;
}

/****************************************************************************
* 名称 ：SPIMODE_TX
* 功能 ：设置SPI 为发送模式
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/ 

void SPIMODE_TX()
{
	*pSPI_CTL = *pSPI_CTL|TIMOD_TX;
	asm("ssync;");	
	udelay(0x3ff);
}

/****************************************************************************
* 名称 ：SPIMODE_RX
* 功能 ：设置SPI 为接收模式
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/ 

void SPIMODE_RX()
{
	*pSPI_CTL = *pSPI_CTL&TIMOD_RX;
	asm("ssync;");
	udelay(0x3ff);
}

/****************************************************************************
* 名称 ：SPIMenable
* 功能 ：设置SPI 主器件模式允许
* 入口参数 ：无
* 出口参数 ：无
****************************************************************************/ 

void SPIMenable()
{
	*pSPI_CTL=*pSPI_CTL|SPI_EN ;  
}


