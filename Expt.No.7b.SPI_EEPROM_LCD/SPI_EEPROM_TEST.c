//Program to Write and read from SPI EEPROM 25Cxxx
//Write a string and reat it to display the read data on LCD
#include <p18f4550.h>
#include "vector_relocate.h"

//Declarations for LCD Connection
#define LCD_DATA    PORTD			//LCD data port to PORTD
#define ctrl        PORTE			//LCD control port to PORTE
#define rs          PORTEbits.RE0	//register select signal to RE0
#define rw          PORTEbits.RE1	//read/write signal to RE1
#define en          PORTEbits.RE2	//enable signal to RE2
/*Declare PORT pin to use as chip select/Slave select*/
#define CS_EEPROM_TRIS		TRISCbits.TRISC6
#define CS_EEPROM     		PORTCbits.RC6

/************ SPI EEPROM Commands *******************/
#define EEPROM_CMD_READ     	(unsigned)0b00000011
#define EEPROM_CMD_WRITE    	(unsigned)0b00000010
#define EEPROM_CMD_WRDI    		(unsigned)0b00000100
#define EEPROM_CMD_WREN    		(unsigned)0b00000110
#define EEPROM_CMD_RDSR     	(unsigned)0b00000101
#define EEPROM_CMD_WRSR     	(unsigned)0b00000001

//Function Prototypes
void msdelay (unsigned int time); //Function to generate delay
void SPIinit(void);		//Function to Initialize SPI Module
unsigned char SPISend(unsigned char data); //Function to send data to SPI Slave
unsigned char SPIReceive(); //Function to receive data from SPI Slave
//Function to Write the single byte to SPI EEPROM IC
void EEPROMWriteByte(unsigned char Data, unsigned int Address);
//Funcction to read the single byte from SPI EEPROM IC
unsigned char EEPROMReadByte(unsigned int Address);
void init_LCD(void);					//Function to initialise the LCD		
void LCD_command(unsigned char cmd);	//Function to pass command to the LCD
void LCD_data(unsigned char data);		//Function to write character to the LCD
void LCD_write_string(unsigned char *str);//Function to write string to the LCD

//Start of main program
void main(void)
{ 
unsigned char i=0;
unsigned char rx_data;
unsigned char trans_data[] = "SPI_TEST";
unsigned char msg1[]="SMARTLOGIC TECH";
unsigned char msg2[]="SPI EEPROM TEST";
unsigned char msg3[]="writing data";
unsigned char msg4[]="reading data";
TRISD = 0x00;         //Configuring PORTD as output
TRISE = 0x00;         //Configuring PORTE as output
ADCON1 = 0x0F;
init_LCD();               //Initialize the LCD module
SPIinit(); 				  //Initializing SPI module
	
//////////// SPI EEPROM write demonstration program ////////
//Display Message
LCD_write_string(msg1); 
LCD_command(0xc0);
LCD_write_string(msg2);
msdelay(1000);

//Display Message SPI Writing and Data to be written                           
LCD_command(0x01);
LCD_write_string(msg3); 
LCD_command(0xc0);
LCD_write_string(trans_data);

//Write 8-Characters to SPI EEPROM 
for(i=0;i < 8;i++)
 {
  EEPROMWriteByte(trans_data[i],(int)i);
  msdelay(15);
 }

msdelay(1000);

LCD_command(0x01);
LCD_write_string(msg4); 
LCD_command(0xc0);

//Read 8-Characters from SPI EEPROM 
for(i=0;i< 8;i++)
 {
  	rx_data =  EEPROMReadByte(i);
	LCD_data (rx_data);
	msdelay(15);
 }

while(1);

}

//Function Definitions 
//Function to generate delay
void msdelay (unsigned int time)
{
 unsigned int i, j;
 for (i = 0; i < time; i++)
    for (j = 0; j < 710; j++);/*Calibrated for a 1 ms delay in MPLAB*/
}

//Function to Initialize SPI Module
void SPIinit(void)
{
TRISBbits.TRISB1=0;     //SCK = output	
TRISBbits.TRISB0=1;		//SDI = input
TRISCbits.TRISC7=0;     //SDO = output
PORTBbits.RB1=0;        //Clearing SDO,SCK initially
PORTCbits.RC7=0;      
CS_EEPROM_TRIS=0;          //Chip select pin = Output
CS_EEPROM =1;              //Initially slave not selected

SSPSTATbits.SMP=1; 		//0 = Input data sampled at middle of data output time
SSPSTATbits.CKE=1;      //0 = Transmit occurs on transition from Idle to active clock state
SSPCON1=0X02;           //SPI Master mode, clock = FOSC/64	
SSPCON1bits.SSPEN=1;	//Enabling SPI module
}	

//Function to send data to SPI Slave
unsigned char SPISend(unsigned char data)
{
SSPBUF=data;			//Load SSPBUF with data
while(!SSPSTATbits.BF); //Check for Buffer full status
}

//Function to receive data from SPI Slave
unsigned char SPIReceive()
{
SSPBUF=0x0;    //dummy write to receive data
while(!SSPSTATbits.BF); //Check for Buffer full status
return(SSPBUF);			//return the received data
}		

//Function to Write the single byte to SPI EEPROM IC
void EEPROMWriteByte(unsigned char Data, unsigned int Address)
{
	unsigned char low_address, high_address;
	low_address = (unsigned char)(Address & 0x00ff);     //Separate out lower address
	high_address = (unsigned char)((Address>>8)&0x00ff); //Separate out higher address

    //Send the Wrtie Enable Command to SPI EEPROM
   	CS_EEPROM =0; 		
   	SPISend(EEPROM_CMD_WREN);
    CS_EEPROM =1; 

	//Write the Data to SPI EEPROM
	CS_EEPROM =0;
    SPISend(EEPROM_CMD_WRITE); 	//Send Write Command   
    SPISend(high_address);		//Send Higher Address (A15 - A8)	
	SPISend(low_address);			//Send Lower Address (A7 - A0)
    SPISend(Data);				//Send data to be written
    CS_EEPROM =1;
	msdelay (100);			//Keep Some delay before disabling
    
	//Send the Wrtie Disable Command to SPI EEPROM
   	CS_EEPROM =0; 
   	SPISend(EEPROM_CMD_WRDI);
    CS_EEPROM =1; 
}

//Funcction to read the single byte from SPI EEPROM IC
unsigned char EEPROMReadByte(unsigned int Address)
{
	unsigned char temp;
	unsigned char low_address, high_address;
	low_address = (unsigned char)(Address & 0x00ff); //Separate out lower address
	high_address = (unsigned char)((Address>>8)&0x00ff); //Separate out higher address

    CS_EEPROM =0;
    SPISend(EEPROM_CMD_READ);		//Send Read Command
    SPISend(high_address);		//Send Higher Address (A15 - A8)	
	SPISend(low_address);			//Send Lower Address (A7 - A0)
   	temp = SPIReceive();					//Read the data from specified location
    CS_EEPROM =1;
    
	return temp; //return the received data
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
void LCD_write_string(unsigned char *str)   
{
    int i = 0;
    while (str[i] != 0)
    {
        LCD_data(str[i]);      // sending data on LCD byte by byte
        msdelay(15);
        i++;
    }
}
