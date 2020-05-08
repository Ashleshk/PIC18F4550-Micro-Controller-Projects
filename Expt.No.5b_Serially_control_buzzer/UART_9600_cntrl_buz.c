//Expt.No.5 - Serial Communication with PC
//Includes
#include <p18f4550.h>
#include "vector_relocate.h"

//Variables
#pragma idata
unsigned char string1[]={"\n\rSmart Logic Technologies"};
unsigned char string2[]={"\n\rUSART Test Code"};
unsigned char string3[]={"\n\rPress 1 to turn ON buzzer"};
unsigned char string4[]={"\n\rPress 2 to turn OFF buzzer\n\r"};
unsigned char string5[]={"\n\rRx Tx test complete"};

//Function Prorotypes
void TXbyte(unsigned char data); //To transmit single character
void TXstring (unsigned char *string); //To transmit string

//Start of Main Program
void main()
{
 unsigned char rx_data, i=0;
 TRISCbits.TRISC2=0; // Configure RC2 as output since  buzzer interfaced to RC2 
 TRISCbits.TRISC7=1; // RXD line as input
 TRISCbits.TRISC6=0; // TXD line as output
 SPBRG = 0xE1;
 SPBRGH = 0x04;      // 0x04E1 for 9600 baud 
 TXSTA = 0x24;       // TX enable BRGH=1
 RCSTA = 0x90;       // continuous RX
 BAUDCON = 0x08;     // BRG16 = 1
 PORTCbits.RC2 = 0;	 // Turn OFF Buzzer
 TXstring (string1); // Transmit string 1
 TXstring (string2); // Transmit string 2
 TXstring (string3); // Transmit string 3
 TXstring (string4); // Transmit string 3
 for (i = 0; i < 10; i++)
	{	
 		while(PIR1bits.RCIF==0); //Wait util data from PC is received
 		rx_data = RCREG;		//Read received value
		if (rx_data == '1')		//Check key 1 is pressed
 			PORTCbits.RC2 = 1;	 // Turn OFF Buzzer
 		while(PIR1bits.RCIF==0); //Wait util data from PC is received
 		rx_data = RCREG;		//Read received value
		if (rx_data == '2')     //Check key 1 is pressed
 			PORTCbits.RC2 = 0;   // Turn ON Buzzer
	}	
 TXstring (string5);	// Transmit string 4
 while(1);		//loop forever
}

void TXbyte(unsigned char data)
{
    while(TXSTAbits.TRMT==0);		//wait till transmit buffer is not empty
    TXREG = data;					// Transmit Data
}

void TXstring(unsigned char *string)
{
	unsigned char i=0;
 	for(i=0;string[i]!='\0';i++) //loop till end of the string
 		TXbyte(string[i]);		//Send single character	
}