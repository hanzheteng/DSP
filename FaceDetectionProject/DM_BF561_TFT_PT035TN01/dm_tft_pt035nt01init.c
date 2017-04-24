#include "dm_bf5xx.h"
#include "dm_types.h"

static void lcd_write_cmd(uint8_t addr, uint8_t val)
{
	int_t cmd;
	
	cmd = (addr<<10) | 1<<9 | val;//bit9是读写控制位
	Drv_SPI_write(cmd);
}

void lcd_init(void)
{	
	*pCtrIner_Flag_OE = 0xff && (~SPI_OE); //SPI_switc 允许
	*pCtrOut_Flag_B = 0x07; // 选择SD_SPISS
	
	Drv_config_to_master();
	Drv_config_master_write();
	Drv_SPI_enable();
	
	lcd_write_cmd(0x02, 0x11);
	lcd_write_cmd(0x03, 0xc8);
	lcd_write_cmd(0x04, 0xca);
	lcd_write_cmd(0x05, 0x86); 
	lcd_write_cmd(0x08, 0x08);
	lcd_write_cmd(0x09, 0x40);
	lcd_write_cmd(0x0a, 0x88);
	lcd_write_cmd(0x0b, 0x88);
	lcd_write_cmd(0x0c, 0x20);
	lcd_write_cmd(0x0d, 0x20);
}

