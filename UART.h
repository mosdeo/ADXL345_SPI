unsigned char RxBuf;

void UART_Initial(void)
{
	TMOD|=0x20;// �ϥ�Timer1 mode2
	SCON=0x50; //
	TH1=TL1=253;//Baud Rate = 19200bps 
	TR1=1; /*Timer1 �Ұ�!*/
	TI=1; //�϶ǰe�i�H�ǰe
}