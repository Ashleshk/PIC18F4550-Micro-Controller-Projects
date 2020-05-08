/*This application helps in understanding the USART peripheral in PIC microcontrollers.
		The Terminal settings for PC are
		1.> Baudrate --> 19200
		2.> Parity   --> None
		3.> Databits --> 8
		4.> Stop bits --> 1
/**************************************************/ 
/** I N C L U D E S **/
#include <p18f4550.h>
#include "vector_relocate.h"

/** V A R I A B L E S ***********************/
#pragma idata
unsigned char String[]={"\n\rSmart Logic Technologies \n\rPIC 18F4550 Development Board\n\rUSART Test Code\n\rSend any character to continue\n\r"};
unsigned char String1[]={"\n\rRx Tx test complete\n\r"};

/** P R I V A T E  FUNCTIONS*********************************/
void TXbyte(char data)
{
    while(TXSTAbits.TRMT==0);		//wait tiiltransmit buffer in not empty
    TXREG = data;					// Transmit Data
}//end TXbyte

void main()
{
 unsigned char temp;
 unsigned char i=0;
 SSPCON1 = 0;        // Make sure SPI is disabled	//Refer Datasheet
 TRISCbits.TRISC7=1; // RX
 TRISCbits.TRISC6=0; // TX
 SPBRG = 0x71;
 SPBRGH = 0x02;      // 0x0271 for 48MHz -> 19200 baud XTAL=20MHz, Fosc=48Mhz 
 TXSTA = 0x24;       // TX enable BRGH=1
 RCSTA = 0x90;       // continuous RX
 BAUDCON = 0x08;     // BRG16 = 1
 temp = RCREG;       // Empty buffer
 temp = RCREG;       // Empty buffer
 
 for(i=0;String[i]!='\0';i++)
 {
  TXbyte(String[i]);
 }
 while(PIR1bits.RCIF==0);        //Wait util data from PC is received

 for(i=0;String1[i]!='\0';i++)
 {
  TXbyte(String1[i]);
 }
 while(1);		//loop forever
}


