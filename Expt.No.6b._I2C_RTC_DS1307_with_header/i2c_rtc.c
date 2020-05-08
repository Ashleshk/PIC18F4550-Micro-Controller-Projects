#include "p18f4550.h"
#include "I2C.h"
#include "vector_relocate.h"

//LCD data pins connected to PORTD and control pins connected to PORTE
#define LCD_DATA    PORTD               //LCD data port
#define en          PORTEbits.RE2      // enable signal
#define rw          PORTEbits.RE1      // read/write signal
#define rs          PORTEbits.RE0     // register select signal

//Function to generate delay
void myMsDelay (unsigned int time)
{
        unsigned int i, j;
        for (i = 0; i < time; i++)
                for (j = 0; j < 710; j++);/*Calibrated for a 1 ms delay in MPLAB*/
}

//Function to pass command to the LCD
void LCD_cmd(unsigned char cmd)
{
    LCD_DATA = cmd;
    rs = 0;
    rw = 0;
    en = 1;
    myMsDelay(15);
    en = 0;
    myMsDelay(15);
    return;
}

//Function to write data to the LCD
void LCD_write(unsigned char data)
{
    LCD_DATA = data;
    rs = 1;
    rw = 0;
    en = 1;
    myMsDelay(15);
    en = 0;
    myMsDelay(15);
    return ;
}
// Function to initialise the LCD
void init_LCD(void)
{
    LCD_cmd(0x38);      // initialization of 16X2 LCD in 8bit mode
    myMsDelay(15);

    LCD_cmd(0x01);      // clear LCD
    myMsDelay(15);

    LCD_cmd(0x0C);      // cursor off
    myMsDelay(15);
    
	LCD_cmd(0x80);      // go to first line and --0 is for 0th position
    myMsDelay(15);    
    return;
}
//Function to split the string into individual characters and call the LCD_write function
void LCD_write_string(static char *str)   //store address value of the string in pointer *str
{
    int i = 0;
    while (str[i] != 0)
    {
        LCD_write(str[i]);      // sending data on LCD byte by byte
        myMsDelay(15);
                i++;
    }
    return;
}

void ascii_display (unsigned char rtc_data)
{
unsigned char temp;
temp=rtc_data;          //Converting Hours from BCD to ascii
temp=temp&0xF0;
temp=(temp>>4)|0x30;
LCD_write (temp);
temp=rtc_data;
temp=(temp&0x0F);
temp=temp|0x30;
LCD_write (temp);
}

void main(void)
{ 
unsigned char i,j;
unsigned char val;
unsigned char msg1[]="SMARTLOGIC TECH";
unsigned char msg2[]="I2C RTC TEST";
unsigned char msg3[]="writing to RTC";
unsigned char msg4[]="Date n Time";
//RTC_Register_MAP: //SS:MM:HH Day, Date, Month, Year
unsigned char RTC_REG[]={0x50,0x59,0x71,0x02,0x21,0x08,0x14,'\0'}; 
						 
TRISD = 0x00;         //Configuring PORTD as output
TRISE = 0x00;         //Configuring PORTE as output

ADCON1=0x0F;              //make ALL PORT PIN as digital 

init_LCD();       //Initialize the LCD module
I2C_init();		  //Initializing I2C module		

//////////// RTC write demonstration programs (DS1307) ////////
//Display Messages
LCD_write_string(msg1);
LCD_cmd(0xc0);
LCD_write_string(msg2);
myMsDelay(500);                              
LCD_cmd(0x01);
LCD_write_string(msg3); 
LCD_cmd(0xc0);
LCD_write_string(msg4);
myMsDelay(500);

//Configure RTC
for (i=0;i < 7; i++)
{	
I2C_byte_write (0xD0,i, RTC_REG[i]);
myMsDelay(20);
}
LCD_cmd(0x01);
while(1)
{
	LCD_cmd(0x80);

	RTC_REG[2] = I2C_byte_read (0xD0,0x02);
	val = RTC_REG[2] & 0x1F;		//Ignore AM/PM n 12/23 info
	ascii_display (val);			//Display Hours
	LCD_write (':');
	
	RTC_REG[1] = I2C_byte_read (0xD0,0x01);
	ascii_display (RTC_REG[1]);		//Display Minutes
	LCD_write (':');
	
	RTC_REG[0] = I2C_byte_read (0xD0,0x00);
	ascii_display (RTC_REG[0]);		//Display Seconds
	
	
	val=RTC_REG[2]&0x20;       //Getting AM or PM from hours location   
	LCD_write (' ');
	if(val)
 		LCD_write ('P');
	else
 		LCD_write ('A');
 		LCD_write ('M');
	
	LCD_cmd (0xC0);				//Go to Second line of LCD
	
	RTC_REG[4] = I2C_byte_read (0xD0,0x04);
	ascii_display (RTC_REG[4]);	//Display Date
	
	LCD_write ('/');
	RTC_REG[5] = I2C_byte_read (0xD0,0x05);
	ascii_display (RTC_REG[5]);   //Display Month
	LCD_write ('/');
	LCD_write ('2');
	LCD_write ('0');
	RTC_REG[6] = I2C_byte_read (0xD0,0x06);
	ascii_display (RTC_REG[6]);	//Display Year
	myMsDelay(250);

}

}

