#include <REG_MG82FL524-564.H>
#include <STDIO.H>
#include "UART.h"
#include "SPI.h"

void delay1ms(unsigned int x)
{unsigned int i,j;
	
	for(i=0;i<x;++i)
		for(j=0;j<1990;++j);
}

unsigned int  ReadData,WriteData,Address;
unsigned char RWmask = 0x80;
unsigned char RxBuf;

int main()
{
	UART_Initial();
	EA=ES=1;
	
	delay1ms(2);
	
	while(1)
	{
		if(0x00 == RWmask)//if Write Mode
		{
			SPI_Write(Address,WriteData);
			
			RWmask = 0x80;//back to Read Mode
		}
		else//if Read Mode
		{
			ReadData = SPI_Read(Address);
			printf("Address = %d, ReadData = %d\r\n",Address, ReadData);
		}
		
		delay1ms(10);
	}
}

void UART_ISR(void) interrupt 4
{
	static char Num;
	
	if(RI)
	{
		RI=0;
		RxBuf=SBUF;
		
		if(RxBuf>96 && RxBuf<123)RxBuf-=32; //¤p¼gÅÜ¤j¼g
		
		switch(RxBuf)
		{
			case 'R':
				RWmask = 0x80;
				break;
			
			case 'W':
				RWmask = 0x00;
				break;
			
			case 'A':
				Address = Num;
				Num = 0;
				break;
			
			case 'D':
				WriteData = Num;
				Num = 0;	
				break;
			
			default:
				if(RxBuf>47 && RxBuf<58)//0~9
				{
					RxBuf-=48; //ASCII to Dec
					Num = Num*10;
					Num = Num + RxBuf;
				}
				break;
		}
	}
}