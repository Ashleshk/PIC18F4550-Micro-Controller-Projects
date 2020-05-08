//Expt.No.5 - Serial Communication with PC
//Includes
#include <p18f4550.h>
#include "vector_relocate.h"

//Variables
#pragma idata
unsigned char string1[]={"\n\rSmart Logic Technologies"};
unsigned char string2[]={"\n\rUSART Test Code"};
unsigned char string3[]={"\n\rSend 10 character to uC\n\r"};
unsigned char string4[]={"\n\rTransmitted Characters are:"};
unsigned char string5[]={"\n\rRx Tx test complete\n\r"};

//Function Prorotypes
void TXbyte(unsigned char data); //To transmit single character
void TXstring (unsigned char *string); //To transmit string

//Start of Main Program
void main()
{
 unsigned char  i=0;
 unsigned char rx_data [20]; //Buffer to store received data
 TRISCbits.TRISC7=1; // RXD line as input
 TRISCbits.TRISC6=0; // TXD line as output
 SPBRG = 0xE1;
 SPBRGH = 0x04;      // 0x04E1 for 9600 baud 
 TXSTA = 0x24;       // TX enable BRGH=1
 RCSTA = 0x90;       // continuous RX
 BAUDCON = 0x08;     // BRG16 = 1
 
 TXstring (string1); // Transmit string 1
 TXstring (string2); // Transmit string 2
 TXstring (string3); // Transmit string 3
 for (i=0; i<10; i++)
	{	
 		while(PIR1bits.RCIF==0); //Wait util data from PC is received
			 rx_data [i]= RCREG; // Read the received data
    }   
 rx_data [10]= 0; // To make received string as ASCIIZ string
TXstring (string4);	// Transmit string 4
TXstring (rx_data);	// Transmit received data
TXstring (string5);	// Transmit string 5
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