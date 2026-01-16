/*
 * Keylogger_atmega32.cpp
 *
 * Created: 2012-03-29 21:05:38
 *  Author: Max
 
 PS2 Cable.
 
 ORANGE = CLOCK
 White = GND
 ¨blue = VCC
 */ 


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay_basic.h>
#include <avr/eeprom.h> 
#include <math.h>
#define F_CPU 10000000UL
#include <util/delay.h>

#include "main.h"
#include "lcd.h"
#include "Menu.h"
#include "F-Bus.h"
#include "usart.h"


extern void MenuNavigation();

structTimer cTimer;

ISR(INT0_vect);
sComputer Brain;
extern sMenu Menu;

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

extern unsigned char RecipientNo[12];
extern unsigned char initPhone(void);

#define VPERADC 0.01953125
#define DEBUG

uint8_t EEMEM SektionsUsed;

int countADC = 1;  /* We'll start with the first ADC channel. 1-8 */
struct sSektion{
	int uID;
	double Voltage;
	char Status;
}Sektion[8];	

float Voltage=0;

ISR(ADC_vect)
{
	Voltage  = (float)ADCH * (float)VPERADC;
	Sektion[countADC].uID = countADC;  /* Helps me keep track of which section is doing what. */
				
	Sektion[countADC].Voltage = Voltage;
	if(Voltage > 4.05 && Voltage < 4.6) //DRIFT
	{
		Sektion[countADC].Status |= stateOK;
		Sektion[countADC].Status &= ~stateALARM;
		Sektion[countADC].Status &= ~stateSABOTAGE;
	}		
	else if(Voltage<4.0 && Voltage > 3) 
	{
		Sektion[countADC].Status &= ~stateOK;
		Sektion[countADC].Status |= stateALARM;
		Sektion[countADC].Status &= ~stateSABOTAGE;
	}
	else //sabotage
	{
		Sektion[countADC].Status &= ~stateOK;
		Sektion[countADC].Status &= ~stateALARM;
		Sektion[countADC].Status |= stateSABOTAGE;
	}		
	  
		

	if(countADC < 8)
	{
		countADC++;
	
	}	
	else
	{
		countADC = 1;
	}		


	SFIOR |= (1 << ACME);  //Set ACME bit in ADCSRB (Enable ADC Multiplexing)
	ADCSRA &= ~1 << ADEN;	//Clear ADEN bit in ADCSRA (TURN ADC OFF)

	switch(ADMUX)
	{

	case 0x20:
		ADMUX = 0x21;
		//ADMUX |= (0 << MUX2) | (0 << MUX1) | (1 << MUX0); //enable ADC1 - 001
		break;
	case 0x21:
		ADMUX = 0x22;
		//ADMUX |= (0 << MUX2) | (1 << MUX1) | (0 << MUX0); //enable ADC2 - 010
		break;
	case 0x22:
		ADMUX = 0x23;
		//ADMUX |= (0 << MUX2) | (1 << MUX1) | (1 << MUX0); //enable ADC3 - 100
		break;
	case 0x23:
		ADMUX = 0x24;
		//ADMUX |= (1 << MUX2) | (0 << MUX1) | (0 << MUX0); //enable ADC4 - 100
		break;
	case 0x24:
		ADMUX = 0x25;
		//ADMUX |= (1 << MUX2) | (0 << MUX1) | (1 << MUX0); //enable ADC5 - 101
		break;
	case 0x25:
		ADMUX = 0x26;
		//ADMUX |= (1 << MUX2) | (1 << MUX1) | (0 << MUX0); //enable ADC6 - 110
		break;
	case 0x26:
		ADMUX = 0x27;
		//ADMUX |= (1 << MUX2) | (1 << MUX1) | (1 << MUX0); //enable ADC7 - 110
		break;
	case 0x27:
		ADMUX = 0x20;
		//ADMUX |= (0 << MUX2) | (0 << MUX1) | (0 << MUX0); //enable ADC0 - 000
		break;
	}
	//ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescalar to 128 - 125KHz sample rate @ 16MHz
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // Set ADC prescalar to 64 - 125KHz sample rate @ 8MHz

	SFIOR |= (1 << ACME);  //Set ACME bit in ADCSRB (Enable ADC Multiplexing)
	//ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	ADCSRA |= (1 << ADEN);  // Enable ADC
   	ADCSRA |= (1 << ADIE);	//Enable ADC interrupt
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
	
	Voltage = 0;
}
char Key=0xFF;
/*
char GetCharPressed()
{
	return 0x00;
	char rtn;
	char r = 0;
	char c = 0;

	KEYPAD_PORT|= 0X0F; //0000 1111

	r = 0;
	c = 0;
	rtn = 0xFF;
	_delay_ms(10);
	for(c=0;c<3;c++)
	{
		KEYPAD_DDR &= ~(0X7F); //0111 1111 Set all DDR to outputs

		KEYPAD_DDR|=(0X40>>c); //0100 0000
		_delay_ms(10);
		for(r=0;r<4;r++)
		{
			//If keypadpin in col c and row r is low, then
			if(!(KEYPAD_PIN & (0X08>>r))) //0000 1000
			{
				_delay_ms(10);
				//return row cTimers 3 + column. example. row=0, col=0;
				//
				rtn = (char)(r*3+c);
				return (char)(r*3+c);
			}
		}
	}

	return 0XFF;//Indicate No key pressed
}
*/
	int test1 = 11111;
	int test2 = 22222;
	int test3 = 3;
	double test4 = 4.3;

int main(void)
{ 	

	//PrintToDisplay("%d%d \n %d%d", test1, test2, test3, test4);
	Menu.mainPos = 1;
	Menu.subPos =	0;
	Menu.DisplayZoneHelpText = true;
	Menu.iZoneHelpTextCount = 0;

	
	Brain.state |= stateOK;

	_delay_ms(50);
    lcd_init(LCD_DISP_ON);
	_delay_ms(50);
	_delay_ms(50);
    lcd_clrscr();
	_delay_ms(50);
	Brain.oldlcdRow1[0] = 0x50;
	Brain.oldlcdRow2[0] = 0x50;
	PrintToDisplay((char*)"|01->00| SYSTEM OK\n  2012-12-08 17:19");
	Setup();
	
//	PrintToDisplay("%d%d \n %d %f", test1, test2, test3, test4);
/*
	UpdateDisplay("Booting up system", (char*)".", printDELAY0);
	int n;
	for(int i = 0; i < 60; i++)
	{
		_delay_ms(1);
		n++;
		if(n>3)
		{
			sprintf(Brain.lcdRow2, "%s.", Brain.lcdRow2);
			UpdateDisplay("Booting up system", Brain.lcdRow2, printDELAY0);
			n=0;
		}
		
	}*/

	Brain.alarmingSections = 0;

while(1)
    {
		_delay_ms(2);
		ScanSections(10);
	
		SetLedStatus();	//Update indicator leds according to current status
		DisplayMenu();
    }
}


char ScanSections(int Section) // 10 to scan all sections.
{
	if(Section == 10)
	{
		Brain.countOK=0;
		Brain.countAlarm=0;
		Brain.countSabotage=0;
		
		for(int i=1; i < 9; i++)
		{

			if((Sektion[i].Status & stateOK) == stateOK)
			{
					Brain.countOK++;
			}
			if((Sektion[i].Status & stateSABOTAGE) == stateSABOTAGE)			
			{
					Brain.countSabotage++;
			}			
			if((Sektion[i].Status & stateALARM) == stateALARM)	
			{		
					Brain.countAlarm++;
			}			
						
		}		
		if(Brain.countOK == 8)
		{
			Brain.state |= stateOK;
			Brain.state &= ~stateSABOTAGE; 
			Brain.state &= ~stateALARM; 
		}	
		if(Brain.countSabotage > 0)
		{
			Brain.state |= stateSABOTAGE;
		}
		if(Brain.countAlarm > 0)
		{
			Brain.state |= stateALARM;
		}					
	
	}
	else
	{
		if((Sektion[Section].Status & stateOK) == stateOK)
		{
			return stateOK;
		}
		if((Sektion[Section].Status & stateSABOTAGE) == stateSABOTAGE)
		{
			return stateSABOTAGE;
		}
		if((Sektion[Section].Status & stateALARM) == stateALARM)
		{
			return stateALARM;
		}
		
	}	
	return stateSABOTAGE;		
}	

/*
uint8_t Key=0xFF;
void Meny()
{
	if((Brain.state & stateOK) != stateOK)
		return;

	Key=0xFF;
	Key = GetKeyPressed();
	Key++;
	
	if(Key!=0xFF)
	{
		if(Key==8)
		{
			if(Brain.Menu < 3)
				Brain.Menu++;
		}else if(Key==2) 
		{
			if(Brain.Menu > 0)
				Brain.Menu--;
		}else if (Key==4) 
		{
			if(Brain.underMenu > 1)
				Brain.underMenu--;
			else if(Brain.underMenu<1)
				Brain.underMenu=8;
			else
				Brain.underMenu=8;
		
		}else if (Key==6)
		{
			if(Brain.underMenu < 8)
				Brain.underMenu++;
			else if (Brain.underMenu>8)
				Brain.underMenu=1;
			else
				Brain.underMenu = 1;
		}					
	}	

	switch(Brain.Menu)
		{
			case menuOVERSIKT:
					sprintf(Brain.lcdRow1, "Sektioner i drift:%d", Brain.countOK);
					sprintf(Brain.lcdRow2, "Larm:%d : Sabotage:%d", Brain.countAlarm, Brain.countSabotage);
					
					
				break;
			case menuLARMER:
					sprintf(Brain.lcdRow1, "Larmer(%d) och Sab(%d)", Brain.countAlarm, Brain.countSabotage);
					if ((Sektion[Brain.underMenu].Status & stateALARM) == stateALARM)
						sprintf(Brain.lcdRow2, "Sek:%d.S (st:%d V:%f)", Brain.underMenu, (int)Sektion[Brain.underMenu].Status, Sektion[Brain.underMenu].Voltage);
					else if ((Sektion[Brain.underMenu].Status & stateSABOTAGE) == stateSABOTAGE)
						sprintf(Brain.lcdRow2, "Sek:%d.S (st:%d V:%f)", Brain.underMenu, (int)Sektion[Brain.underMenu].Status, Sektion[Brain.underMenu].Voltage);
					else if((Sektion[Brain.underMenu].Status & stateOK) == stateOK)
						sprintf(Brain.lcdRow2, "Sek:%d - OK V:%d", Brain.underMenu, (int)Sektion[Brain.underMenu].Voltage);
					else
						sprintf(Brain.lcdRow2, "Tekniskt fel.");
				break;
				
			default:
					
				break;
		}		
		
		UpdateDisplay(Brain.lcdRow1, Brain.lcdRow2, printdelay500);					
	
}	*/

void Setup(void)
{
/*------------------------------------------------------------------------------
	 cTimer STUFF
Set up the cTimer, prescaler of clksource (1/F_CPU per second) */
	TCCR0|=(1<<CS02)|(1<<CS00); // Set up cTimer
	TIMSK|=(1<<TOIE0);
	TCNT0=0;
	cTimer.countcTimerFlipMenuText			=	0;
	cTimer.countcTimerDisplayLoginScreen	=	0;
	cTimer.countcTimerReadKeyPress			=	0;
	cTimer.countcTimerAutoLogOut			=	0;
	cTimer.countcTimerPrintTimeout			=	0;
	
	cTimer.bPrintTimeout					= false;
	cTimer.bAutoLogOut						= false;
	cTimer.bDisplayLoginText				= false;	
	cTimer.bPrintNextMenuItem				= false;
	cTimer.bReadKeyPress					= true;
/*		Prescaler: 
	1024 / 8000000 = 0,000128 one tick
	2 / 0,000128 = 15625 ticks for 2 seconds.
------------------------------------------------------------------------------
	 END cTimer STUFF */
	Menu.clockPos=0;
	Menu.bSettingClock = false;
	Brain.bLoggedIn = false;
	Brain.bLoginState = false;
	strcpy(Brain.correctPassword, "1234");
	
	Brain.lcdRow1[0] = 0;
	Brain.lcdRow2[0] = 0;
	
	
    //uint8_t SRAMchar;
    //SRAMchar = eeprom_read_byte(&SektionsUsed); 
	
	
	DDRD |= 1 << 7; /* Red Led Pin */
	DDRC |= 1 << 0; /* Greem LED pin */
	DDRC |= 1 << 1; /* Orange LED pin */
	

	ADMUX |= (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // Set ADC prescalar to 8 - 125KHz sample rate @ 16MHz


	ADMUX &= ~1 << REFS0; // Set ADC reference to AVCC
	ADMUX &= ~1 << REFS1;
	SFIOR |= (1 << ACME);  //Set ACME bit in ADCSRB (Enable ADC Multiplexing)
	ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

	ADCSRA |= (1 << ADEN);  // Enable ADC
   	ADCSRA |= (1 << ADIE);	//Enable ADC interrupt
	ADCSRA |= (1 << ADSC);  // Start A2D Conversions 
	
	sei();				//Enable global interrupts
}	

void SetLedStatus()
{
	
	if((Brain.state & stateOK) == stateOK)
		PORTC |= 1<<1;		//Green 
	else
		PORTC &= ~1<<1;
		
	if((Brain.state & stateSABOTAGE) == stateSABOTAGE)
		PORTC |= 1<<0;
	else
		PORTC &= ~1<<0;
	
	if((Brain.state & stateALARM) == stateALARM)
		PORTD |= 1<<7;
	else
		PORTD &= ~1<<7;
}	


int AlarmingSections(char section, char type)
{ /* Not in use atm */
	if(type==stateALARM || type==stateSABOTAGE)
	{
		switch(section)
		{
			case 1:
			Brain.alarmingSections |= SECTION1;
			break;
			case 2:
			Brain.alarmingSections |= SECTION2;
			break;
			case 3:
			Brain.alarmingSections |= SECTION3;
			break;
			case 4:
			Brain.alarmingSections |= SECTION4;
			break;
			case 5:
			Brain.alarmingSections |= SECTION5;
			break;
			case 6:
			Brain.alarmingSections |= SECTION6;
			break;
			case 7:
			Brain.alarmingSections |= SECTION7;
			break;
			case 8:
			Brain.alarmingSections |= SECTION8;
			break;
			
			default:
			break;
		}
	}else if(type==stateOK)
	{
		switch(section)
		{
			case 1:
			Brain.alarmingSections &= ~SECTION1;
			break;
			case 2:
			Brain.alarmingSections &= ~SECTION2;
			break;
			case 3:
			Brain.alarmingSections &= ~SECTION3;
			break;
			case 4:
			Brain.alarmingSections &= ~SECTION4;
			break;
			case 5:
			Brain.alarmingSections &= ~SECTION5;
			break;
			case 6:
			Brain.alarmingSections &= ~SECTION6;
			break;
			case 7:
			Brain.alarmingSections &= ~SECTION7;
			break;
			case 8:
			Brain.alarmingSections &= ~SECTION8;
			break;
			
			default:
			break;
		}
	}
	return 0xff;
}

ISR(TIMER0_OVF_vect)
{
	/* cTimer0 overflow interrupt 
		cTimer configured at  
	Prescaler: 1024
	1024 / 8000000 = 0,000128 one tick
 Clock input of cTimer0 = 16MHz/1024 = 15625 Hz Frequency of Overflow = 15625 /256 = 61.0352 Hz 
 if we increment a variable “count” every Overflow when “count reach 61” approx one second has elapse. 
	*/ 
	
	cTimer.countcTimerFlipMenuText++;
	if(cTimer.countcTimerFlipMenuText>122) // 2 seconds)
	{
		cTimer.countcTimerFlipMenuText=0;
		cTimer.bPrintNextMenuItem = !cTimer.bPrintNextMenuItem;
	}
	
	cTimer.countcTimerReadKeyPress++;
	if(cTimer.countcTimerReadKeyPress>3 || cTimer.bReadKeyPress == false)  //~305ms. 0.061ms / integer increment
	{
		cTimer.countcTimerReadKeyPress = 0;
		cTimer.bReadKeyPress = true;
	}
	
	cTimer.countcTimerPrintTimeout++;
	if(cTimer.countcTimerPrintTimeout>6)  //~305ms. 0.061ms / integer increment
	{
		cTimer.countcTimerPrintTimeout = 0;
		cTimer.bPrintTimeout = true;
	}
	
	cTimer.countcTimerDisplayLoginScreen++;
	if(cTimer.countcTimerDisplayLoginScreen>122)
	{
		cTimer.countcTimerDisplayLoginScreen = 0;
		cTimer.bDisplayLoginText = true;
	}
	
	cTimer.countcTimerAutoLogOut++;
	if(cTimer.countcTimerAutoLogOut>1000)		//use 18300 - 5 minutes
	{
		cTimer.countcTimerAutoLogOut = 0;
		cTimer.bAutoLogOut = true;
	}
}