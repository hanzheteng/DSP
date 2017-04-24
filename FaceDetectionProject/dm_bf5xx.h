#ifndef __BF5XX_H__
#define __BF5XX_H__

#include <stdio.h>
#include <cdefBF53x.h>
#include <sys\exception.h>

#include "dm_types.h"

#define SCL		0x0001		// PF0
#define SDA		0x0002		// PF1

#define CLKIN	  (30.0e6)		// clockin frequency in Hz
#define CORECLK  (270.0e6)		// core clock frequency in Hz
#define SYSCLK	 (54.0e6)		// system clock frequency in Hz

#define CLR_PF(pf) ((*pFIO0_FLAG_C = pf))
#define SET_PF(pf) ((*pFIO0_FLAG_S = pf))
#define READ_PF_STAT(pf) ((pf = *pFIO0_FLAG_D))

#define TIMOD_RX	0xfffc
#define TIMOD_TX	0x0001
#define SLAVE		0x0010
#define MASTER		0x1000
#define MISO		0x0020
#define SPI_EN		0x4000

#define ADT7516_WRITE		0x90  //Ð´ÃüÁî
#define ADT7516_READ		0x91  //¶ÁÃüÁî

#define DACALSB				0x10
#define DACAMSB				0x11
#define ADT7516_CONFIG1		0x18
#define ADT7516_DID			0x4d
#define ADT7516_MID			0x4e
#define ADT7516_SPISTATUS	0x7f

#define MCP2515_WRITE   	0x02
#define MCP2515_READ    	0x03
#define MCP2515_RESET   	0xc0
#define MCP2515_BITMOD      0x05
#define RD_STAT				0xA0 

#define BFPCTRL             0x0c
#define TXRTSCTRL           0x0d
#define CANSTAT             0x0e
#define CANCTRL             0x0f
#define CNF3                0x28
#define CNF2                0x29
#define CNF1                0x2A
#define CANINTE             0x2B
#define CANINTF             0x2C
#define EFLG                0x2D

#define TXB0CTRL            0x30
#define TXB0SIDH            0x31
#define TXB0SIDL            0x32
#define TXB0EID8            0x33
#define TXB0EID0            0x34
#define TXB0DLC             0x35
#define TXB0D0              0x36
#define TXB0D1              0x37
#define TXB0D2              0x38
#define TXB0D3              0x39
#define TXB0D4              0x3A
#define TXB0D5              0x3B
#define TXB0D6              0x3C
#define TXB0D7              0x3D

#define RXB0CTRL            0x60
#define RXB0SIDH            0x61
#define RXB0SIDL            0x62
#define RXB0EID8            0x63
#define RXB0EID0            0x64
#define RXB0DLC             0x65
#define RXB0D0              0x66
#define RXB0D1              0x67
#define RXB0D2              0x68
#define RXB0D3              0x69
#define RXB0D4              0x6A
#define RXB0D5              0x6B
#define RXB0D6              0x6C
#define RXB0D7              0x6D

#define RXF0SIDH            0x00
#define RXF0SIDL            0x01
#define RXF0EID8            0x02
#define RXF0EID0            0x03

#define RXF1SIDH            0x04
#define RXF1SIDL            0x05
#define RXF1EID8            0x06
#define RXF1EID0            0x07

#define TIMOD_R				0x0000
#define TIMOD_T				0x0001
#define TIMOD_RDMA			0x0010
#define TIMOD_TDMA			0x0011
	
#define NAND_ALE 			0x0800
#define NAND_CLE 			0x0400
#define NAND_CE				0x0200
#define NAND_RDY 			0x0100

/////////////////////////////PCMCIA//////////////////////
#define CF_Busy_FLAG  0x8000
#define CF_RDY_FLAG   0x4000
#define CF_DWF_FLAG   0x2000
#define CF_DSC_FLAG   0x1000
#define CF_DRQ_FLAG   0x0800
#define CF_CORR_FLAG  0x0400
#define CF_IDX_FLAG   0x0200
#define CF_ERR_FLAG   0x0100
#define PIOMODE		  0x02
#define MEMMODE		  0x00
#define CF_SRESET 0x80
#define Base_Addr  0x2c040000

// ½øÈëI/OÄ£Ê½ÏÂ Ê×ÏÈÅäÖÃÈý¸ö¼Ä´æÆ÷ µØÖ·¶¨ÒåÈçÏÂ 

#define ConfigOption_Reg				(volatile unsigned short*)(Base_Addr+0x200)
#define CardStatus_Reg 					(volatile unsigned short*)(Base_Addr+0x202)
#define CardCopy_Reg 					(volatile unsigned short*)(Base_Addr+0x206)

#define DATA_REGISTER 					(volatile unsigned short*)(Base_Addr+0x1f0)   //Êý¾Ý¼Ä´æÆ÷
#define ERROR_REGISTER 					(volatile unsigned short*)(Base_Addr+0x1f0)  //´íÎó¼Ä´æÆ÷
#define SECTOR_REG 						(volatile unsigned short*)(Base_Addr+0x1f2) //ÉÈÇøÊý¼Ä´æÆ÷
#define SECTOR_COUNT_REG 				(volatile unsigned short*)(Base_Addr+0x1f2) //ÉÈÇøÊý¼Ä´æÆ÷
#define SECTOR_NUMBER_REG 				(volatile unsigned short*)(Base_Addr+0x1f2)  //ÉÈÇøºÅ¼Ä´æÆ÷
#define CYLINDER_REG 					(volatile unsigned short*)(Base_Addr+0x1f4)   //ÖùÃæµÍÎ»¼Ä´æÆ÷
#define CYLINDER_LOW_REG 				(volatile unsigned short*)(Base_Addr+0x1f4)   //ÖùÃæµÍÎ»¼Ä´æÆ÷
#define CYLINDER_HI_REG 				(volatile unsigned short*)(Base_Addr+0x1f4)     //ÖùÃæ¸ßÎ»¼Ä´æÆ÷
#define DRIVE_HEAD_REG 					(volatile unsigned short*)(Base_Addr+0x1f6)       //Çý¶¯´Å¿¨²åÍ·¼Ä´æÆ÷
#define STATUS_REGISTER 				(volatile unsigned short*)(Base_Addr+0x1f6)      //×´Ì¬¼Ä´æÆ÷
#define COMMAND_REGISTER 				(volatile unsigned short*)(Base_Addr+0x1f6)//ÃüÁî¼Ä´æÆ÷
#define AltStatus_REGISTER 				(volatile unsigned short*)(Base_Addr+0x3f6)
#define DeviceControl_REGISTER 			(volatile unsigned short*)(Base_Addr+0x3f6)
#define DriveAddress_REGISTER 			(volatile unsigned short*)(Base_Addr+0x3f6)
#define RESET_REG 						(volatile unsigned short*)(Base_Addr+0x0200)
//#define CAPTURE_MODE_SXGA       //¶¨ÒåÉãÏñÍ·²É¼¯Í¼Æ¬µÄ´óÐ¡Îª1280*1024
#define SENSOR_REG_SIZE  217
#ifdef CAPTURE_MODE_SXGA
	#define PIXEL_PER_LINE 1280  // # of samples expected from the sensor
	#define LINES_PER_FRAME 1024 // # of lines expected from the sensor
	#define PPICOUNT 2559
#else	
	#define PIXEL_PER_LINE 320  // # of samples expected from the sensor
	#define LINES_PER_FRAME 240 // # of lines expected from the sensor
	#define PPICOUNT 639
#endif

#define Out_Flag_OE  0xff
#define Iner_Flag_OE 0xFF;
#define Out_Flag_A   0x00
#define Out_Flag_B   0x00
#define Out_Flag_C   0x00
#define DMA_EN       0x0001
#define TMR8_EN      0x0001
#define TMR9_EN      0x0002



//////////////////////////////////////////////////////////
//                       cpld address map         
/////////////////////////////////////////////////////////
#define BANK3_ADDR_BASE  0x2c000000

#define IDECS0BASEADDR	(BANK3_ADDR_BASE+0x00000)
#define IDECS1BASEADDR	(BANK3_ADDR_BASE+0x10000)
#define DM9000BASEADDR	(BANK3_ADDR_BASE+0x20000)
#define USBCSBASEADDR	(BANK3_ADDR_BASE+0x30000)
#define PCMCIABASEADDR	(BANK3_ADDR_BASE+0x40000)

#define pIDECS0BASEADDR	(volatile uint16_t *)IDECS0BASEADDR
#define pIDECS1BASEADDR	(volatile uint16_t *)IDECS1BASEADDR
#define pDM9000BASEADDR	(volatile uint16_t *)DM9000BASEADDR
#define pUSBCSBASEADDR	(volatile uint16_t *)USBCSBASEADDR
#define pPCMCIABASEADDR	(volatile uint16_t *)PCMCIABASEADDR

#define pNAND_BaseaAddr		(volatile unsigned char *)0x2c3b0000
//FLASH Register Address
#define pFlash_Data		(volatile unsigned char *)(pNAND_BaseaAddr +0x00)
#define pFlash_Commond		(volatile unsigned char *)(pNAND_BaseaAddr +0x08)
#define pFlash_Addr		(volatile unsigned char *)(pNAND_BaseaAddr + 0x04)

#define  NAND_RDY		0x0100
	
#define DM9000_PPTR   *(volatile uint16_t *)(DM9000BASEADDR)
#define DM9000_PDATA  *(volatile uint16_t *)(DM9000BASEADDR + 0x10)

#define pCtrOut_Flag_OE	(volatile uint8_t *)(BANK3_ADDR_BASE+0x50000)
#define MOTOR_OE    0x01
#define TFT_245_OE  0x02
#define CMOS_245_OE 0x04
#define PS2_245_OE  0x08

#define LED_OE      0x20
#define DMB_OE      0x40
#define IRDA_SD     0x80

#define LeftBtn   0x01
#define RightBtn  0x02
#define Middle    0x04
#define Xsign     0x10
#define Ysign     0x20
#define Xoverflow 0x40
#define Yoverflow 0x80

#define pCtrIner_Flag_OE (volatile uint16_t *)(BANK3_ADDR_BASE+0x60000)
#define MOUSE_SW_OE 0x01
#define KEY_SW_OE   0x02
#define LEDCS       0x04
#define SPI_OE      0x08
#define PCM_MEM     0x10
#define PCM_IO	    0x20


#define pCtrOut_Flag_A (volatile uint16_t *)(BANK3_ADDR_BASE+0x70000)
#define MOTOR_A     0x01
#define MOTOR_B     0x02
#define MOTOR_C     0x04
#define MOTOR_D     0x08
#define PS2_245_DIR 0x10
#define PS_DAT_DIR  0x20
#define CMOS_RST    0x40

#define pCtrOut_Flag_B (volatile uint16_t *)(BANK3_ADDR_BASE+0x80000)
#define SPI_A0      0xC1
#define SPI_A1      0xC2
#define SPI_A3      0xC4
#define LED_A0      0xD0
#define LED_A1      0xE0
#define CMOSCLK_SET 0x40
#define TFTCLK_SET  0x80

#define pLED_DAT_OUT   (volatile uint16_t *)(BANK3_ADDR_BASE+0x90000)
#define pCtrOut_Flag_C (volatile uint16_t *)(BANK3_ADDR_BASE+0xA0000)
#define USB_D_SUS     0x01
#define USB_H_SUS     0x02
#define KEY_OUT_CLK   0x04
#define MOUSE_OUT_CLK 0x08
#define KEY_SW        0x10
#define MOUSE_SW      0x20
#define AD_LDAC       0x80

/************************************************************/
/*               I/O  map               				   	*/
/************************************************************/

#define pCtrIn_Int_A (volatile uint16_t *)(BANK3_ADDR_BASE+0xC0000)
#define ISP1362_INT2  0x01
#define ISP1362_INT1  0x02
#define PCMCIA_INT    0x04
#define DM9000_INT    0x08
#define ADDA_INT      0x10
#define ADS_INT       0x20
#define CAN_INT       0x40
#define DMB_INT0      0x80

#define pCtrIn_Int_B (volatile uint16_t *)(BANK3_ADDR_BASE+0xD0000)
#define DMB_INT1      0x01
#define Mouse_INT     0x02
#define Keyy_INT      0x04
#define SD_INF        0x08
#define ADS_BUSY      0x10
#define DM9000_IOWAIT 0x20
#define PCMCIA_WAIT   0x40
#define PCMCIA_CD     0x80

#define pCtrIn_Kb (volatile uint16_t *)(BANK3_ADDR_BASE+0xE0000)

//--------------------------------------------------------------------------//
// platform Support															//
//--------------------------------------------------------------------------//
static inline void udelay(volatile int_t us)
{
	volatile int_t i;
	
	while(us--)
		for(i=6; i>0; i--);
}


static inline void mdelay(volatile int_t ms)
{
	while(ms--)
		udelay(300);
}

//--------------------------------------------------------------------------//
// LED Support																//
//--------------------------------------------------------------------------//
#define	LED13 (0x0001)
#define	LED14 (0x0002)
#define	LED15 (0x0004)	
#define	LED16 (0x0008)
#define	LED17 (0x0010)
#define	LED18 (0x0020)
#define	LED19 (0x0040)
#define	LED20 (0x0080)

inline void	Setup_LEDs() { 	*pFIO2_DIR    = 0x00ff;	}
inline void Leda_Off()   {	*pFIO2_FLAG_C = 0x00ff; }
										
inline void	Led_On(uint16_t LED) {	*pFIO2_FLAG_S = LED;	}
inline void	Led_Off(uint16_t LED){	*pFIO2_FLAG_C = LED;	}

static inline void Set_CE(bool state)
{
	if(!state)
	{	
		*pFIO2_FLAG_C =  NAND_CE;
	}
	else
	{
		*pFIO2_FLAG_S |= NAND_CE;
	}
}

static inline void Set_CLE(bool state)
{
	if(!state)
	{
		*pFIO2_FLAG_C = NAND_CLE;
	}
	else
	{
		*pFIO2_FLAG_S |= NAND_CLE;
	}
}

static inline void Set_ALE(bool state)
{
	if(!state)
	{
		*pFIO2_FLAG_C =  NAND_ALE;
	}
	else
	{
		*pFIO2_FLAG_S |= NAND_ALE;	
	}
}



//LED_CPLD
static inline void Init_LED_CPLD(void)
{  
    *pCtrOut_Flag_OE =(0xff&(~LED_OE));
    mdelay(500);
}
//UART_CPLD
static inline void init_UART_CPLD(void)
{
	*pCtrOut_Flag_OE = (0xff&(~IRDA_SD));		//ÖÃµÍIRDAµÄSD½Å
	mdelay(500);
}
//CAN_CPLD
static inline void init_CAN_CPLD(void)
{
 	*pCtrOut_Flag_B   = SPI_A0;//0x01; // Ñ¡ÔñCAN_SPISS
 	mdelay(500);
}





#endif 











