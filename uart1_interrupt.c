#include "headerfile.h"
extern s8 rfidID[12];
volatile int i=0;

void uart1_handler(void)__irq
{
	int t=U1IIR;
	
	if(t&4)
  	{
		rfidID[i++]=U1RBR;
   	} 	
	VICVectAddr=0;
}

void config_VIC(void)
{
	VICIntSelect=0;
	VICVectCntl1=7|(1<<5);
	VICVectAddr1=(int)uart1_handler;
	VICIntEnable|=1<<7;
}

void en_uart1_intr(void)
{
	U1IER=3;
}



