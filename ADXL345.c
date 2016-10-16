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
unsigned char RxBuf;

int DATAX[2],DATAY[2],DATAZ[2];
int* ptrDATA[] = {&DATAX[0],&DATAX[1],&DATAY[0],&DATAY[1],&DATAZ[0],&DATAZ[1]};
int test = -4095;

int accX, accY, accZ;


void ShowConfig()
{
	int i;
	
	for(i=0;i<=57;i++)
	{
		if( 1<=i && i<=28)continue;	
		printf("Address = %d, Value = %X\n\r", i, (unsigned int)SPI_Read(i));
	}
}

int main()
{
	UART_Initial();
	ShowConfig();
	
	//EA=ES=1;
	//EA=EX0=1;
	//IT0=1; //nINT0 Falling Edge Trigger
	
	delay1ms(2);
	SPI_Write(0x31,0x0B);
	SPI_Write(0x2D,0x08);
	SPI_Write(0x2E,0x80);
	
	delay1ms(12);
	SPI_Write(0x1E,0x00); //OFSX
	SPI_Write(0x20,0x80); //OFSZ
	
	while(1)
	{
		char i; 
		
		//for(i=0;i<6;i++)
		for(i=5;i>=0;i--)
		{
			*ptrDATA[i] = (int)SPI_Read(i+50);
		}
		
		accX = ((DATAX[1]<<8)&0xFF00) | (DATAX[0]&0xFF);
		accY = ((DATAY[1]<<8)&0xFF00) | (DATAY[0]&0xFF);
		accZ = ((DATAZ[1]<<8)&0xFF00) | (DATAZ[0]&0xFF);
		
		printf("%d, %d, %d,\n",accX, accY, accZ);
		delay1ms(5);
	}
}

// void INT0_ISR(void) interrupt 0
// {
// 	char i;
// 	unsigned char tempSDO;
// 	ET0=0;
// 	
// 	SPI_CS(0);
// 	
// 	for(i=7;i>=0;i--)
// 	{
// 		//F-Edge
// 		SPI_SCL(1);
// 		SDA = 0x1&((0xF2)>>i);
// 		SPI_SCL(0);
// 	}
// 	
// 	//===========================
// 	SPI_SCL(1);                                                         
// 	//===========================
// 	
// 	for(i=7;i>=0;i--)
// 	{
// 		//R-Edge
// 		SPI_SCL(0);
// 		SPI_SCL(1);
// 		tempSDO = SDO; //Read bit
// 		
// 		DATAX[0] |= tempSDO<<i;
// 	}
// 	
// 	//===========================
// 	SPI_SCL(1);                                                         
// 	//===========================
// 	
// 	for(i=7;i>=0;i--)
// 	{
// 		//R-Edge
// 		SPI_SCL(0);
// 		SPI_SCL(1);
// 		tempSDO = SDO; //Read bit
// 		
// 		DATAX[1] |= tempSDO<<i;
// 	}
// 	
// 	SPI_CS(1);
// 	NewData = 1;
// 	ET0=1;
// }

// void UART_ISR(void) interrupt 4
// {
// 	static char Num;
// 	
// 	if(RI)
// 	{
// 		RI=0;
// 		RxBuf=SBUF;
// 		
// 		if(RxBuf>96 && RxBuf<123)RxBuf-=32; //¤p¼gÅÜ¤j¼g
// 		
// 		switch(RxBuf)
// 		{
// 			case 'R':
// 				//RWmask = 0x80;
// 				break;
// 			
// 			case 'W':
// 				//RWmask = 0x00;
// 				break;
// 			
// 			case 'A':
// 				Address = Num;
// 				Num = 0;
// 				break;
// 			
// 			case 'D':
// 				WriteData = Num;
// 				Num = 0;	
// 				break;
// 			
// 			default:
// 				if(RxBuf>47 && RxBuf<58)//0~9
// 				{
// 					RxBuf-=48; //ASCII to Dec
// 					Num = Num*10;
// 					Num = Num + RxBuf;
// 				}
// 				break;
// 		}
// 	}
// }