#include <REG_MG82FL524-564.H>
#include <INTRINS.H>

#define SDO P34
#define SDA P35
#define SCL P36
#define CS  P37

void SPI_CS(bit isSelect)
{
	CS=isSelect;
	_nop_();
}

void SPI_SCL(bit Level)
{
	SCL=Level;
	_nop_();
}

unsigned char SPI_Read(unsigned char Address)
{
	unsigned char ReadData=0;
	unsigned char tempSDO;
	char i;
	
	SPI_CS(0);
	
	for(i=7;i>=0;i--)
	{
		//F-Edge
		SPI_SCL(1);
		SDA = 0x1&((0x80|Address)>>i);
		SPI_SCL(0);
	}
	
	//===========================
	SPI_SCL(1);
	//===========================
	
	for(i=7;i>=0;i--)
	{
		//R-Edge
		SPI_SCL(0);
		SPI_SCL(1);
		tempSDO = SDO; //Read bit
		
		ReadData |= tempSDO<<i;
	}
	
	SPI_CS(1);
	
	ReadData &= 0xFF;
	
	return ReadData;
}

void SPI_Write(unsigned char Address, unsigned char WriteData)
{
	char i;
	
	SPI_CS(0);
	
	for(i=7;i>=0;i--)
	{
		//F-Edge
		SPI_SCL(1);
		SDA = 0x1&((0x7F&Address)>>i);
		SPI_SCL(0);
	}
	
	for(i=7;i>=0;i--)
	{
		//F-Edge
		SPI_SCL(1);
		SDA = 0x1&((WriteData)>>i);
		SPI_SCL(0);
	}
	
	SPI_CS(1);
}
	