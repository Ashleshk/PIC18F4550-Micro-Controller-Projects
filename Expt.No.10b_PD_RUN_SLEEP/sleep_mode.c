//Expt. - Observing supply current by using power saving modes

//Connect multimeter or ammeter at JP1 located in power section
//connection if external power is used - JP1 -pinno.1 and pinno.2
//connection if USB power is used - JP1 -pinno.2 and pinno.3

#include <p18f4550.h>
#include "vector_relocate.h"


//Function Prototypes
void msdelay (unsigned int time);//Function for delay

//Start of Program Code
void main()					//Main Program
{
	TRISD = 0x00;			//To configure PORTD (LED) as output	
	
	while (1)				//While loop for repeated operation	
	{
	PORTD = 0x01;			//Turn ON first LED (L0) to indicate RUN MODE
	msdelay (4000);
	
	OSCCONbits.IDLEN = 0;	//Setting for SLEEP MODE 
	PORTD = 0x02;			//Turn ON third LED (L2) to indicate SLEEP Mode
	_asm
		sleep				//Execute SLEEP instruction to move from IDLE to SLEEP
	_endasm	

	}		

} 							

//Function Definitions
void msdelay (unsigned int time)//Function for delay
{
unsigned int i, j;
 for (i = 0; i < time; i++)
	 for (j = 0; j < 710; j++);	//Calibrated for a 1 ms delay in MPLAB
}




