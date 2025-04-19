#include <stdio.h>
#include "headerfile.h"
s8 Buffer[33];
s8 rfidID[12];
extern int i;

int main() 
{
	u8 rtcTIME[12];  // HH:MM:SS DD/MM/YY
	u8 h,m,s,D,M,Y,a;
	
	uart1_init(9600);
	i2c_init();
	lcd_init();
	config_VIC(); 
  en_uart1_intr();

// 	i2c_byte_write_frame(0xD0,0x2,0x41);	// Set hours
// 	i2c_byte_write_frame(0xD0,0x1,0x28);	// Set minutes
// 	i2c_byte_write_frame(0xD0,0x0,0x00);	// Set seconds
// 	i2c_byte_write_frame(0xD0,0x4,0x29);	// Set date
// 	i2c_byte_write_frame(0xD0,0x5,0x02);	// Set month
// 	i2c_byte_write_frame(0xD0,0x6,0x24);	// Set years
	
	lcd_cmd(0x80);
	lcd_string((s8*)"SHOW YOUR CARD");
	
    while(1) 
	{    
		h=i2c_byte_read_frame(0xD0,0x2);
		m=i2c_byte_read_frame(0xD0,0x1);
		s=i2c_byte_read_frame(0xD0,0x0);

		a=h&0x1f;

	   	D=i2c_byte_read_frame(0xD0,0x4);
		M=i2c_byte_read_frame(0xD0,0x5);
		Y=i2c_byte_read_frame(0xD0,0x6);
	    
		lcd_cmd(0xC0);
		lcd_data((a/0x10)+48);
		lcd_data((a%0x10)+48);
		lcd_data(':');
		lcd_data((m/0x10)+48);
  	lcd_data((m%0x10)+48);
		lcd_data(':');
		lcd_data((s/0x10)+48);
  	lcd_data((s%0x10)+48);

		if((h>>5&1)==1)
		{
         lcd_string((s8*)"PM "); 
		}
		else
		{
				 lcd_string((s8*)"AM ");
		}
		
		  lcd_data((D/0x10)+48);
	    lcd_data((D%0x10)+48);
	    lcd_data('/');
	    lcd_data((M/0x10)+48);
	    lcd_data((M%0x10)+48);

	  rtcTIME[0] = (a/0x10)+48; 		
		rtcTIME[1] = (a%0x10)+48; 	
		rtcTIME[2] = (m/0x10)+48; 		
		rtcTIME[3] = (m%0x10)+48;		
		rtcTIME[4] = (s/0x10)+48; 
		rtcTIME[5] = (s%0x10)+48;
		rtcTIME[6] = (D/0x10)+48; 
		rtcTIME[7] = (D%0x10)+48;
		rtcTIME[8] = (M/0x10)+48; 
		rtcTIME[9] = (M%0x10)+48;
		rtcTIME[10] = (Y/0x10)+48; 
		rtcTIME[11] = (Y%0x10)+48;

		if(i>11)
		{
		sprintf(Buffer,"%s %c%c:%c%c:%c%c %c%c/%c%c/%c%c\r\n",rfidID, rtcTIME[0], rtcTIME[1], rtcTIME[2], rtcTIME[3], rtcTIME[4], rtcTIME[5], 
																		rtcTIME[6], rtcTIME[7] ,rtcTIME[8] ,rtcTIME[9] ,rtcTIME[10] ,rtcTIME[11]);
		i=0;
		uart1_tx_string(Buffer);
		}	
		
	}
}	




 



