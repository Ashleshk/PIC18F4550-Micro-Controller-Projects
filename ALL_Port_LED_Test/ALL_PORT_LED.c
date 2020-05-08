//Test_Program: Interfacing LEDs to each port one by one
//Includes
#include <p18f4550.h>			//Include Controller specific .h
#include "vector_relocate.h"	//Vector Remapping for USB HID Bootloader

//Function Prototypes
void msdelay (unsigned int time);//Function for delay

//Start of Program Code
void main()						//Main Program
{
	
    INTCON2bits.RBPU=0;   		//To Activate the internal pull on PORTB
	ADCON1 = 0x0F;				//To disable the all analog inputs	
	
	TRISA = 0x00;				//To configure PORTA  as output
	TRISB = 0x00;				//To configure PORTB  as output
	TRISC = 0x00;				//To configure PORTC  as output
	TRISD = 0x00;				//To configure PORTD  as output	
	TRISE = 0x00;				//To configure PORTE  as output
	
	

while (1)						//While loop for repeated operation
	{
		PORTA = 0xAA;
		PORTB = 0xAA;
		PORTC = 0xAA;
		PORTD = 0xAA;
		PORTE = 0xAA;
		
		msdelay(250);

		PORTA = 0x55;
		PORTB = 0x55;
		PORTC = 0x55;
		PORTD = 0x55;
		PORTE = 0x55;
		
		msdelay(250);
	  
	}
} 								//End of the Program

//Function Definitions
void msdelay (unsigned int time)//Function for delay
{
unsigned int i, j;
 for (i = 0; i < time; i++)
	 for (j = 0; j < 710; j++);	//Calibrated for a 1 ms delay in MPLAB
}




