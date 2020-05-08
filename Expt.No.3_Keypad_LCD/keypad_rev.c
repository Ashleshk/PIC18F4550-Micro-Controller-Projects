//Expt.3: 4x4 Matrix Keypad Interfacing
//Includes
#include <p18f4550.h>
#include "vector_relocate.h"

//Declarations
#define LCD_DATA    PORTD			//LCD data port to PORTD
#define ctrl        PORTE			//LCD control port to PORTE
#define rs          PORTEbits.RE0	//register select signal to RE0
#define rw          PORTEbits.RE1	//read/write signal to RE1
#define en          PORTEbits.RE2	//enable signal to RE2

//Function Prototypes
void init_LCD(void);					//Function to initialise the LCD		
void LCD_command(unsigned char cmd);	//Function to pass command to the LCD
void LCD_data(unsigned char data);		//Function to write character to the LCD
void LCD_write_string(static char *str);//Function to write string to the LCD
void msdelay (unsigned int time);		//Function to generate delay

//Start of Main Program
void main()
{
	char  msg1 [] = "SMART LOGIC TECH";
	char  msg2 [] = "4x4 KEYPAD TEST";
	char  msg3 [] = "Press Key";
	char  msg4 [] = "Pressed Key:";
    int key;
	    
    INTCON2bits.RBPU=0; //To enable the internal pullup
    TRISB = 0xF0; 		//PORTB lower nibble as O/P & higher nibble as I/P.
    TRISD = 0x00; 		//PORTD as output port
	TRISE  =0x00;		//PORTE as output port
	ADCON1 = 0x0F; 		//PORTE as digital input
	
	init_LCD();          // initialization of LCD	
	LCD_write_string (msg1); //Display msg1 on LCD
	LCD_command (0xC0);		//Move LCD cursor to second line
	LCD_write_string (msg2); //Display msg2 on LCD
	msdelay(350);      	//  Call delay function
	LCD_command (0x01);		//Clear LCD
	LCD_write_string (msg3); //Display msg3 on LCD
	LCD_command (0xC0);			//Move LCD cursor to second line
	LCD_write_string (msg4); //Display msg4 on LCD
	msdelay(350);      // Call delay function
    while(1)
      {
		//Row0 made 0 and columns checked for keypress     
        PORTB = 0x0E;
		msdelay(10);
        switch(PORTB & 0xF0)
		{
        	case 0xE0:
				msdelay (100); 	//Key debouncing time
				if((PORTB & 0xF0) == 0xE0)//Check Whether key from COl0
					{
                		key='0';		  // means the MS0('0') is pressed
						LCD_command(0xCC); // Move LCD curcsor to 2nd line - 13th position
						LCD_data (key);	  //Display keycode on LCD
					}
                    break;

        	case 0xD0:
                msdelay (100);			//Key debouncing time
				if((PORTB & 0xF0) == 0xD0)//Check Whether key from COl1
					{
                		key='1';		  // means the MS1 ('1') is pressed
						LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
						LCD_data (key);	  //Display keycode on LCD
					}
                    break;

            case 0xB0:
				msdelay (100);			  //Key debouncing time
				if((PORTB & 0xF0) == 0xB0)//Check Whether key from COl2
				{
                	key='2';			// means the MS2 ('2') is pressed
					LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
					LCD_data (key);		//Display keycode on LCD
				}
			    break;

            case 0x70:					
				msdelay (100);				//Key debouncing time
				if((PORTB & 0xF0) == 0x70) //Check Whether key from COl3 
				{
                    key='3';				// means the MS3 ('3') is pressed
					LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
					LCD_data (key);		//Display keycode on LCD
				}
                 break;
		}
		
		//Row1 made 0 and columns checked for keypress
        PORTB = 0x0D;
		msdelay(10);
        switch(PORTB & 0xF0)
           {
            case 0xE0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xE0)	//Check Whether key from COl0
			{
	            key='4';				// means the MS4 ('4') is pressed
				LCD_command(0xCC);		// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);			//Display keycode on LCD
			}
            break;

            case 0xD0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xD0)	//Check Whether key from COl1
			{
	            key='5';			// means the MS5 ('5') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
			}
            break;

            case 0xB0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xB0)	//Check Whether key from COl2
			{
            	key='6';			// means the MS6 ('6') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
			}
          	break;

            case 0x70:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0x70)	//Check Whether key from COl3
			{
           		key='7';			// means the MS7 ('7') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
		 	}   break;
            }
	//Row2 made 0 and columns checked for keypress     
        PORTB = 0x0B;
		msdelay(10);
        switch(PORTB & 0xF0)
		{
        	case 0xE0:
				msdelay (100); 	//Key debouncing time
				if((PORTB & 0xF0) == 0xE0)//Check Whether key from COl0
					{
                		key='8';			// means the MS8 ('8') is pressed
						LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
						LCD_data (key);		//Display keycode on LCD
					}
                    break;

        	case 0xD0:
                msdelay (100);				//Key debouncing time
				if((PORTB & 0xF0) == 0xD0)//Check Whether key from COl1
					{
                		key='9';			// means the MS9 ('9') is pressed
						LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
						LCD_data (key);		//Display keycode on LCD
					}
                    break;

            case 0xB0:
				msdelay (100);				//Key debouncing time
				if((PORTB & 0xF0) == 0xB0)	//Check Whether key from COl2
				{
                	key='A';			// means the MS10 ('A') is pressed
					LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
					LCD_data (key);		//Display keycode on LCD
				}
			    break;

            case 0x70:
				msdelay (100);				//Key debouncing time
				if((PORTB & 0xF0) == 0x70)	//Check Whether key from COl3
				{
                    key='B';			// means the MS11 ('B') is pressed
					LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
					LCD_data (key);		//Display keycode on LCD
				}
                 break;
		}

		//Row3 made 0 and columns checked for keypress
        PORTB = 0x07;
		msdelay(10);
        switch(PORTB & 0xF0)
        	{
            case 0xE0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xE0)	//Check Whether key from COl0
			{
	            key='C';			// means the MS12 ('C') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
			}
            break;

            case 0xD0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xD0)	//Check Whether key from COl1
			{
	            key='D';			// means the MS13 ('D') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
			}
            break;

            case 0xB0:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0xB0)	//Check Whether key from COl2
			{
            	key='E';			// means the MS14 ('E') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
			}
          	break;

            case 0x70:
			msdelay (100);				//Key debouncing time
			if((PORTB & 0xF0) == 0x70)	//Check Whether key from COl3
			{
           		key='F';			// means the MS15 ('F') is pressed
				LCD_command(0xCC);	// Move LCD curcsor to 2nd line - 13th position
				LCD_data (key);		//Display keycode on LCD
		 	}   break;
            }

			
        }
}

//Function Definitions
void msdelay (unsigned int time) //Function to generate delay
{
unsigned int i, j;
  for (i = 0; i < time; i++)
	for (j = 0; j < 710; j++);//Calibrated for a 1 ms delay in MPLAB
}


void init_LCD(void)		// Function to initialise the LCD
{
    LCD_command(0x38);      // initialization of 16X2 LCD in 8bit mode
    msdelay(15);
    LCD_command(0x01);      // clear LCD
    msdelay(15);
    LCD_command(0x0C);      // cursor off
    msdelay(15);
    LCD_command(0x80);      // go to first line and 0th position
    msdelay(15);
}

void LCD_command(unsigned char cmd) //Function to pass command to the LCD
{
    LCD_DATA = cmd;		//Send data on LCD data bus
    rs = 0;				//RS = 0 since command to LCD
    rw = 0;				//RW = 0 since writing to LCD
    en = 1;				//Generate High to low pulse on EN
    msdelay(15);
    en = 0;
}

void LCD_data(unsigned char data)//Function to write data to the LCD
{
    LCD_DATA = data;	//Send data on LCD data bus
    rs = 1;				//RS = 1 since data to LCD
    rw = 0;				//RW = 0 since writing to LCD
    en = 1;				//Generate High to low pulse on EN
  	msdelay(15);
    en = 0;
}
//Function to write string to LCD
void LCD_write_string(static char *str)   
{
    int i = 0;
    while (str[i] != 0)
    {
        LCD_data(str[i]);      // sending data on LCD byte by byte
        msdelay(15);
        i++;
    }
}
