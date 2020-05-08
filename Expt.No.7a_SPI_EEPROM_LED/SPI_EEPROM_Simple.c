//Program to Write and read from SPI EEPROM 25Cxxx
//Write a character and read it to display the read data on LED
#include <p18f4550.h>
#include "vector_relocate.h"

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

//Start of Main Program
void main(void)
{ 
unsigned char rx_data;

TRISD = 0x00;         //Configuring PORTD as output
PORTD = 0x00;         //Initially PORTD made low
SPIinit(); 			  //Initializing SPI module
	
EEPROMWriteByte(0x0f, 0x0010);	//Write Data to SPI EEPROM
msdelay(500);

rx_data =  EEPROMReadByte(0x0010);	//Read Data from SPI EEPROM
PORTD = rx_data;					//Verify the data using PORTD

while(1);   //Endless loop
}   //End of main program

//Function Definitions 
//Function to generate delay
void msdelay (unsigned int time)
{
 unsigned int i, j;
 for (i = 0; i < time; i++)
    for (j = 0; j < 710; j++);/*Calibrated for a 1 ms delay in MPLAB*/
}

//Function to Initialize SPI Module
void SPIinit()
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