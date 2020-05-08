//Expt.No.6: Interfacing I2C based RTC (DS1307) to PIC Microcontroller 
#include "p18f4550.h"
#include "vector_relocate.h"

//Declarations
#define Brd_Fosc 48000000
#define I2C_CLOCK   100E3
#define SSPADD_VAL  ((Brd_Fosc/(4*I2C_CLOCK))-1)
#define LCD_DATA    PORTD               //LCD data port
#define en          PORTEbits.RE2      // enable signal
#define rw          PORTEbits.RE1      // read/write signal
#define rs          PORTEbits.RE0     // register select signal


//Function Prototypes
void I2C_init(void);					//Function to initialise I2C module
unsigned char I2C_byte_write(unsigned char device,unsigned char address,unsigned char data);
unsigned char I2C_byte_read (unsigned char device,unsigned char address);
void init_LCD(void);					//Function to initialise the LCD		
void LCD_command(unsigned char cmd);	//Function to pass command to the LCD
void LCD_data(unsigned char data);		//Function to write character to the LCD
void LCD_write_string(static char *str);//Function to write string to the LCD
void msdelay (unsigned int time);		//Function to generate delay
void ascii_display (unsigned char rtc_data); //Function to convert BCD to ascii


//Start of Main Program
void main(void)
{ 
unsigned char i,j;
unsigned char val;
unsigned char msg1[]="SMARTLOGIC TECH";
unsigned char msg2[]="I2C RTC TEST";
unsigned char msg3[]="writing to RTC";
unsigned char msg4[]="Date n Time";
//RTC_Register_MAP:     //SS:MM:HH Day, Date, Month, Year
unsigned char RTC_REG[]={0x50,0x59,0x71,0x02,0x21,0x08,0x14,'\0'}; 
						 
TRISD = 0x00;         //Configuring PORTD as output
TRISE = 0x00;         //Configuring PORTE as output

ADCON1=0x0F;          //make ALL PORT PIN as digital 

init_LCD();       //Initialize the LCD module
I2C_init();		  //Initializing I2C module		

//////////// RTC write demonstration programs (DS1307) ////////
//Display Messages
LCD_write_string(msg1);
LCD_command(0xc0);
LCD_write_string(msg2);
msdelay(500);                              
LCD_command(0x01);
LCD_write_string(msg3); 
LCD_command(0xc0);
LCD_write_string(msg4);
msdelay(500);

//Configure RTC
for (i=0;i < 7; i++)
{	
I2C_byte_write (0xD0,i, RTC_REG[i]);
msdelay(20);
}
LCD_command(0x01);
while(1)
{
	LCD_command(0x80);

	RTC_REG[2] = I2C_byte_read (0xD0,0x02);
	val = RTC_REG[2] & 0x1F;		//Ignore AM/PM n 12/23 info
	ascii_display (val);			//Display Hours
	LCD_data (':');
	
	RTC_REG[1] = I2C_byte_read (0xD0,0x01);
	ascii_display (RTC_REG[1]);		//Display Minutes
	LCD_data (':');
	
	RTC_REG[0] = I2C_byte_read (0xD0,0x00);
	ascii_display (RTC_REG[0]);		//Display Seconds
	
	
	val=RTC_REG[2]&0x20;       //Getting AM or PM from hours location   
	LCD_data (' ');
	if(val)
 		LCD_data ('P');
	else
 		LCD_data ('A');
 		LCD_data ('M');
	
	LCD_command (0xC0);				//Go to Second line of LCD
	
	RTC_REG[4] = I2C_byte_read (0xD0,0x04);
	ascii_display (RTC_REG[4]);	//Display Date
	
	LCD_data ('/');
	RTC_REG[5] = I2C_byte_read (0xD0,0x05);
	ascii_display (RTC_REG[5]);   //Display Month
	LCD_data ('/');
	LCD_data ('2');
	LCD_data ('0');
	RTC_REG[6] = I2C_byte_read (0xD0,0x06);
	ascii_display (RTC_REG[6]);	//Display Year
	msdelay(25);

}

}

//Function Definitions
void I2C_init()
{
TRISBbits.TRISB0=1;	//Setting up SDA/SCL pins as inputs
TRISBbits.TRISB1=1;	
SSPCON1 = 0x08;		//I2C Master mode, clock = FOSC/(4 * (SSPADD + 1))
SSPSTATbits.SMP=1;  //Enabling slew control for high sped mode
SSPADD= SSPADD_VAL; //Setting I2C clock
SSPCON1bits.SSPEN=1;//Enabling MSSP module
}	

unsigned char I2C_byte_write(unsigned char device,unsigned char address,unsigned char data)
{
SSPCON2bits.SEN=1;           //Generating start condition
while(!PIR1bits.SSPIF);      //Waiting for transmission of start bit
PIR1bits.SSPIF=0;            //Clearing SSPIF flag

SSPBUF=device&0xFE;          //Sending Device Address
while(!PIR1bits.SSPIF);      //Waiting for transmission of device address
PIR1bits.SSPIF=0;            //Clearing SSPIF flag 

SSPBUF=address;				 //Sending Memory Location
while(!PIR1bits.SSPIF);      //Waiting for transmission of memory address
PIR1bits.SSPIF=0;            //Clearing SSPIF flag 
 
SSPBUF=data;
while(!PIR1bits.SSPIF);
PIR1bits.SSPIF=0;  
 
SSPCON2bits.PEN=1;        //Generating STOP condition
while(!PIR1bits.SSPIF);   //Waiting for transmission of stop bit
PIR1bits.SSPIF=0;         //Clearing SSPIF flag	     
}


unsigned char I2C_byte_read(unsigned char device,unsigned char address)
{
unsigned char data;
SSPCON2bits.SEN=1;           //Generating start condition
while(!PIR1bits.SSPIF);      //Waiting for transmission of start bit
PIR1bits.SSPIF=0;            //Clearing SSPIF flag

SSPBUF=device&0xFE;          //Selecting device in write mode

while(!PIR1bits.SSPIF);      //Waiting for transmission of device address along with reception of ACK bit
PIR1bits.SSPIF=0;            //Clearing SSPIF flag 
  
SSPBUF=address;              //Setting read address
while(!PIR1bits.SSPIF);
PIR1bits.SSPIF=0;  
 
SSPCON2bits.RSEN=1;          //generating repeated start condition
while(!PIR1bits.SSPIF);
PIR1bits.SSPIF=0;	  
 
////Sending Read Command
SSPBUF=device|0x01;          //Sending Address
while(!PIR1bits.SSPIF);      //Waiting for transmission of read command along with reception of ACK bit
PIR1bits.SSPIF=0;            //Clearing SSPIF flag 

SSPCON2bits.RCEN=1;          //Enabling Master Receive Mode
while(!PIR1bits.SSPIF);      //Waiting to receive a byte
PIR1bits.SSPIF=0;            //Clearing interrupt flag 	 
data=SSPBUF;                //Getting received data byte  

SSPCON2bits.PEN=1;           //Generating STOP condition
while(!PIR1bits.SSPIF);
PIR1bits.SSPIF=0;	 
return data;
}

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

void ascii_display (unsigned char rtc_data)
{
unsigned char temp;
temp=rtc_data;          //Store data Temporarily 
temp=temp&0xF0;			//unpack higher byte 
temp=(temp>>4)|0x30;	//Add 0x30 to unpacked BCD 
LCD_data (temp);		//write converted BCD digit
temp=rtc_data;			//Store data Temporarily  
temp=(temp&0x0F);		//unpack lower byte
temp=temp|0x30;			//Add 0x30 to unpacked BCD 
LCD_data (temp);		//write converted BCD digit
}