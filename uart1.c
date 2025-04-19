#include "headerfile.h"
//UART1
#define UART1 0x50000				//P0.8 P0.9

void uart1_init(u32 baud)
{
	s32 a[]={15,60,30,0,15};
	u32 pclk=a[VPBDIV]*1000000;
	u32 result=0;
	result=pclk/(16*baud);
	PINSEL0|=UART1;
	U1LCR=0x83;
	U1DLL=result&0xFF;								
	U1DLM=(result>>8)&0xFF;
	U1LCR=0x03;
}

void uart1_tx_char(u8 data)
{
	U1THR=data;
	while(THRE==0);
}

void uart1_tx_string(s8 *ptr)
{
	while(*ptr!=0)
	{
		U1THR=*ptr;
		while(THRE==0);
		ptr++;
	}
}

/////////////////////////////////////////////////////////////////////////////

u8 uart1_rx_char(void)
{
	while(RDR==0);
	return U1RBR;
}

void uart1_rx_string(s8 *ptr,s32 max_bytes)
{
	s32 i;
	for(i=0;i<max_bytes;i++)
	{
		while(RDR==0);
		ptr[i]=U1RBR;
		if(ptr[i]=='\r')
			break;
	}
	ptr[i]='\0';
}




