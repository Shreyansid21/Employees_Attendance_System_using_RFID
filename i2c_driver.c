#include "headerfile.h"
//I2C
#define SI ((I2CONSET>>3)&1)

void i2c_init(void)
{
	PINSEL0|=0x50;		//P0.2 SCL	P0.3 SDA
	I2SCLH=75;				//i2c speed 100kbps
	I2SCLL=75;				//i2c speed 100kbps
	I2CONSET=(1<<6);	//enable i2c peripheral
}

void i2c_byte_write_frame(u8 sa,u8 mr,u8 data)
{
	I2CONSET=(1<<5);	//STA=1
	I2CONCLR=(1<<3);	//SI=0
		while(SI==0);					//START CONDITION
	I2CONCLR=(1<<5);	//STA=0
	if(I2STAT!=0x08)
	{
		uart1_tx_string((s8*)"Error:start condition\r\n");
		goto exit;
	}
	
	I2DAT=sa;			//send SA+WR
	I2CONCLR=(1<<3);	//SI=0
		while(SI==0);					//SEND SA+WR FROM MASTER TO SLAVE
	if(I2STAT==0x20)
	{
		uart1_tx_string((s8*)"Error:SA+W\r\n");
		goto exit;		
	}
	
	I2DAT=mr;			//send mr addr.
	I2CONCLR=(1<<3);	//SI=0
		while(SI==0);					//SEND MEMORY ADDR. FROM MASTER TO SLAVE
	if(I2STAT==0x30)
	{
		uart1_tx_string((s8*)"Error:memory address\r\n");
		goto exit;		
	}

	I2DAT=data;			//send data
	I2CONCLR=(1<<3);	//SI=0
		while(SI==0);
	if(I2STAT==0x30)
	{
		uart1_tx_string((s8*)"Error:data\r\n");
		goto exit;
	}

	exit:
		I2CONSET=(1<<4);	//STO=1
		I2CONCLR=(1<<3);	//SI=0		STOP CONDITION
}


u8 i2c_byte_read_frame(u8 sa,u8 mr)
{
	unsigned char temp;
	I2CONSET=(1<<5);		//STA=1
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);					//START CONDITION
	I2CONCLR=(1<<5);		//STA=0
	if(I2STAT!=0x08)
	{
		uart1_tx_string((s8*)"Error:start condition\r\n");
		goto exit;
	}
	
	I2DAT=sa;				//send SA+WR
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);					//SEND MEMORY ADDR. FROM MASTER TO SLAVE
	if(I2STAT==0x20)
	{
		uart1_tx_string((s8*)"Error:SA+W\r\n");
		goto exit;
	}
	
	I2DAT=mr;				//send mr addr.
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);					//SEND MEMORY ADDR. FROM MASTER TO SLAVE
	if(I2STAT==0x30)
	{
		uart1_tx_string((s8*)"Error:memory address\r\n");
		goto exit;
	}
	
	I2CONSET=(1<<5);		//STA=1		RESTART CONDITION
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);					//GENERATE RESTART CONDITION
	I2CONCLR=(1<<5);		//STA=0
	if(I2STAT!=0x10)
	{
		uart1_tx_string((s8*)"Error:restart condition\r\n");
		goto exit;
	}
	
	I2DAT=sa|1;				//send SA+R
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);					//SEND SA+R FROM MASTER TO SLAVE
	if(I2STAT==0x48)
	{
		uart1_tx_string((s8*)"Error:SA+R\r\n");
		goto exit;
	}
	
	/* data read */
	I2CONCLR=(1<<3);		//SI=0
		while(SI==0);		//waiting for data receive
	temp=I2DAT;							//READING 1 BYTE DATA FROM SLAVE TO MASTER
	
	exit:
		I2CONSET=(1<<4);	//STO=1		GENERATE STOP CONDITION
		I2CONCLR=(1<<3);	//SI=0
		return temp;
}









