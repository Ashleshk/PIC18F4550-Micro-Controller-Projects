//Expt.No.4: Generation of Square wave using timer interrupt
//Includes
#include <p18f4550.h>


//Vector Relocation for USB HID Bootloader
extern void _startup (void);
#pragma code _RESET_INTERRUPT_VECTOR = 0x1000
void _reset (void)
{
	_asm 
		goto _startup 
	_endasm
}
#pragma code

#pragma code _LOW_INTERRUPT_VECTOR = 0x1018
 void low_ISR (void)
{}
#pragma code

//Timer ISR Function Prototype
void timer_isr(void);

// Timer ISR Definition
#pragma interrupt timer_isr
void timer_isr(void)
{
	TMR0H = 0X6D;          //Reload the timer values after overflow
	TMR0L = 0X82;
	PORTBbits.RB0 = ~PORTBbits.RB0; //Toggle the RB0
 	INTCONbits.TMR0IF = 0; //Reset the timer overflow interrupt flag
}

//Interrupt Vector for Timer0 Interrupt
#pragma code _HIGH_INTERRUPT_VECTOR = 0x1008
void high_ISR (void)
{
	_asm 
		goto timer_isr //The program is relocated to execute the ISR
	_endasm    
}
#pragma code

// Start of main Program
void main()
{	
	ADCON1 = 0x0F;        //Configuring the PORTE pins as digital I/O 
	TRISBbits.TRISB0 = 0; //Configruing the RB0 as output
	PORTBbits.RB0 = 0;    //Setting the initial value	
	T0CON = 0x03;		  //Set the timer to 16-bit,Fosc/4,1:16 prescaler
  	TMR0H = 0x6D;         //load timer value to genearate delay 50ms
  	TMR0L = 0x82;
   	INTCONbits.TMR0IF = 0;// Clear Timer0 overflow flag
	INTCONbits.TMR0IE = 1;// TMR0 interrupt enabled
 	T0CONbits.TMR0ON = 1; // Start timer0
	INTCONbits.GIE = 1;	  // Global interrupt enabled

	while(1);             //Program execution stays here untill the timer overflow interrupt is generated
	
}
