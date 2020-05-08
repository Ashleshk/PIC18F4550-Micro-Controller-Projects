/*his application helps in understanding the ADC peripheral in PIC microcontrollers.
The analog voltage is applied to analog pin AN1(PORTA - RA1) of the microntroller.
The resultant digital data is put on LCD */
	
		
/** I N C L U D E S ********/
#include <p18f4550.h>
#include "vector_relocate.h"

#define LCD_DATA    PORTD               //LCD data port
#define en          PORTEbits.RE2      // enable signal
#define rw          PORTEbits.RE1      // read/write signal
#define rs          PORTEbits.RE0     // register select signal

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void ADC_Init(void);
void Select_Channel(unsigned char Channel);
unsigned int Get_ADC_Result(void);
void Start_Conversion(void);

void myMsDelay (unsigned int time);
void LCD_cmd(unsigned char cmd);
void init_LCD(void);
void LCD_write(unsigned char data);
void LCD_write_string(static char *str);


void ADC_Init()
{
 ADCON0=0b00000100;	//A/D Module is OFF and Channel 1 is selected
 ADCON1=0b00001110;	// Reference as VDD & VSS, AN0 set as analog pins
 ADCON2=0b10001110; // Result is right Justified
					//Acquisition Time 2TAD
					//ADC Clk FOSC/64
 ADCON0bits.ADON=1; //Turn ON ADC module
}



void Start_Conversion()
{
 ADCON0bits.GO=1;
}


//If you do not wish to use adc conversion interrupt you can use this
//to do conversion manually. It assumes conversion format is right adjusted
unsigned int Get_ADC_Result()
{
 unsigned int ADC_Result=0;
 while(ADCON0bits.GO);
 ADC_Result=ADRESL;
 ADC_Result|=((unsigned int)ADRESH) << 8;
 return ADC_Result;
} 


void myMsDelay (unsigned int time)
{
	unsigned int i, j;
	for (i = 0; i < time; i++)
		for (j = 0; j < 710; j++);/*Calibrated for a 1 ms delay in MPLAB*/
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

    LCD_cmd(0x80);      // ---8 go to first line and --0 is for 0th position
    myMsDelay(15);
    return;
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

void main()
{ 
 char msg1[] = "Smart Logic Tech";
 char msg2[] = "ADC O/P:";
 unsigned char i, Thousands,Hundreds,Tens,Ones;
 unsigned int adc_val;
 unsigned char val, pot0[6];
 ADCON1 = 0x0F;        //Configuring the PORTE pins as digital I/O 
 TRISD = 0x00;         //Configuring PORTD as output
 TRISE = 0x00;         //Configuring PORTE as output
	
 ADC_Init();				// Init ADC peripheral
 init_LCD();				// Init LCD Module
 LCD_write_string(msg1);	// Display Welcome Message
 LCD_cmd (0xC0);			// Goto second line, 0th place of LCD
 LCD_write_string(msg2);	// Display Message "ADC O/P"

 while(1)
 {
  	Start_Conversion();			//Trigger conversion
  	adc_val= Get_ADC_Result();//Get the ADC output by polling GO bit
  
  	LCD_cmd (0xC8);			//Goto 9th place on second line of LCD
  	
	i = adc_val/1000 ;		//Get the thousands place 
  	Thousands = i + 0x30;		// Convert it to ASCII
  	LCD_write (Thousands);	// Display thousands place
  	
	i = (adc_val%1000)/100;	//Get the Hundreds place
  	Hundreds = i + 0x30;		// Convert it to ASCII
  	LCD_write (Hundreds);		//Display Hundreds place
  
	i = ((adc_val%1000)%100)/10; //Get the Tens place
  	Tens = i + 0x30;			// Convert it to ASCII
  	LCD_write (Tens);			//Display Tens place
  	
	i = adc_val%10 ;			//Get the Ones place
  	Ones = i + 30; 			// Convert it to ASCII
  	LCD_write (i + 0x30);		//Display Ones place
 
  	myMsDelay(300);		//Delay between conversions. It is a library function,refer delays.h file in MCC18 installation directory
 }
}
