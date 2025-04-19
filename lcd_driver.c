#include "headerfile.h"
//LCD
#define RS (1<<17)					//P1.17
#define RW (1<<18)					//P1.18
#define EN (1<<19)					//P1.19
#define LCD4 0x00FE0000			//P1.20 P1.21 P1.22 P1.23 

void lcd_data(u8 data)
{
	u32 temp;
	IOCLR1|=LCD4;
	temp=(data&0xF0)<<16;
	IOSET1=temp;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=EN;	
		delay_ms(2);
	IOCLR1=EN;
	
	IOCLR1|=LCD4;
	temp=(data&0x0F)<<20;
	IOSET1=temp;
	IOSET1=RS;
	IOCLR1=RW;
	IOSET1=EN;	
		delay_ms(2);
	IOCLR1=EN;
}

void lcd_cmd(u8 cmd)
{
	u32 temp;
	IOCLR1|=LCD4;
	temp=(cmd&0xF0)<<16;
	IOSET1=temp;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=EN;	
		delay_ms(2);
	IOCLR1=EN;
	
	IOCLR1|=LCD4;
	temp=(cmd&0x0F)<<20;
	IOSET1=temp;
	IOCLR1=RS;
	IOCLR1=RW;
	IOSET1=EN;	
		delay_ms(2);
	IOCLR1=EN;
}

void lcd_init(void)
{
	IODIR1|=LCD4;
	PINSEL2|=0;
	lcd_cmd(0x02);
	lcd_cmd(0x03);
	lcd_cmd(0x28);
	lcd_cmd(0x0C);
	lcd_cmd(0x01);
}

void lcd_string(s8 *ptr)
{
	while(*ptr!=0)
	{
		lcd_data(*ptr);
		ptr++;
		delay_ms(2);
	}
}

void lcd_display(s8 *str, u8 row, u8 col) 
{
    u8 address = col + (row == 1 ? 0x40 : 0x00);
    lcd_cmd(0x80 | address);
    lcd_string(str);
}
