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
	int temp;   	
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

void Drv_config_to_master()
{
	*pFIO0_DIR |= PF2;
	*pSPI_FLG = FLS2;
	// Set baud rate SPI_SCK = HCLK/(2*SPIBAUD) SCK	
	*pSPI_BAUD = 60000;//SPI_CLK = 30MHz
	// configure spi port
	// 8-bit data, MSB first, SPI Master
 

	*pSPI_CTL = MSTR | SIZE;// | CPHA;
}


void Drv_SPI_write(int_t val)
{
	*pFIO0_FLAG_S = PF2;
	while(*pSPI_STAT & TXS);
	*pSPI_FLG = FLG2;
	*pSPI_TDBR = val;
	*pSPI_FLG = FLS2;
}



/****************************************************************************
* ���� ��SPIinit
* ���� ����ʼ��SPI������ ������Ϊ1M ����8λ �����MSB
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/ 

void SPIinit()
{	
	*pSPI_BAUD=1000; 
	*pSPI_FLG=0x0004;  
	*pSPI_CTL = 0x0400;
	// CPOL =0 high active ;CPHA=1; MSB  fist ; size =8 ;MISO=enable;
	*pSPI_CTL|=MSTR|CPOL|EMISO;
}

/****************************************************************************
* ���� ��SPIMODE_TX
* ���� ������SPI Ϊ����ģʽ
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/ 

void SPIMODE_TX()
{
	*pSPI_CTL = *pSPI_CTL|TIMOD_TX;
	asm("ssync;");	
	udelay(0x3ff);
}

/****************************************************************************
* ���� ��SPIMODE_RX
* ���� ������SPI Ϊ����ģʽ
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/ 

void SPIMODE_RX()
{
	*pSPI_CTL = *pSPI_CTL&TIMOD_RX;
	asm("ssync;");
	udelay(0x3ff);
}

/****************************************************************************
* ���� ��SPIMenable
* ���� ������SPI ������ģʽ����
* ��ڲ��� ����
* ���ڲ��� ����
****************************************************************************/ 

void SPIMenable()
{
	*pSPI_CTL=*pSPI_CTL|SPI_EN ;  
}


