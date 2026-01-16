/*
 * Menu.cpp
 *
 * Created: 2012-12-08 16:32:06
 *  Author: Max
 */ 
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay_basic.h>
#include <avr/eeprom.h>
#include <math.h>
#define F_CPU 10000000UL
#include <util/delay.h>

#define PASSWORDOFFSET	9

#include "main.h"
#include "Menu.h"
#include "lcd.h"

char alphabet[30] = {'a','b','c','d','e','f',
'g','h','i','j','k','l','m','n','o',
'p','q','r','s','t','u','v','w','x','y','z',
	'.','/','\\','-'};
char TempLine[21] = {0x50, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64, 0x3a}; //Password:
	
sMenu Menu;

extern sComputer Brain;
extern structTimer cTimer;

extern char ScanSections(int Section);
int letMeLogIn = 0;
int enteredPasswordLength, enteredPasswordLengthBuffer = 0;

char oldTempDispl[45];

void DisplayMenu()
{
	uint8_t tempKey;
	tempKey = 0xFF;

	tempKey = GetKeyPressed();
	if(tempKey!=0xFF)
	{
		tempKey++;
		cTimer.bReadKeyPress = false;
		if(tempKey == 12)
		{	/* RETURN */
			Brain.bLoginState=true;
			Brain.countLoginPass = 0;
			Menu.mainPos = mainMENU8;
		}
	}
	
	MenuNavigation();
	

	PrintMenu();
	
	//if(strcmp(Brain.oldlcdRow1, Brain.lcdRow1)!=0)
	//	strcpy(Brain.oldlcdRow1, Brain.lcdRow1);
	//else if(strlen(Brain.oldlcdRow1))
	//if(strcmp(Brain.oldlcdRow2, Brain.lcdRow2)!=0)
	//	strcpy(Brain.oldlcdRow2, Brain.lcdRow2);
	//if(strcmp(Brain.oldlcdRow1, Brain.lcdRow1)!=0 ||
	//	strcmp(Brain.oldlcdRow2, Brain.lcdRow2)!=0)

}

char PrintMenu()
{
	_delay_ms(10);
	//_delay_us(100);

	uint8_t tempPass = 0xFF;
	uint8_t tmpKey = 0xFF;

	switch(Menu.mainPos)
	{
		case mainMENU1:
		
			if(Menu.subPos == 0)
			{
					if ((Brain.state & stateDEACTIVE) == stateDEACTIVE)
					{
						
						PrintToDisplay((char*)"|01->00| Deactivated\n  2012-12-08 17:19");
					}
					else if ((Brain.state & stateALARM) == stateALARM)
					{
						PrintToDisplay((char*)"|01->00| ALARM [6]->\nAlarms:%d Sabotage:%d", Brain.countAlarm, Menu.subPos);
	
					}else if ((Brain.state & stateSABOTAGE) == stateSABOTAGE)
					{
						// VILKEN SEKTION LARMAR
						PrintToDisplay((char*)"|01->00| SABOTAGE \nAlarms:%d Sabotage:%d", Brain.countAlarm, Brain.countSabotage);
					}else if ((Brain.state & stateOK) == stateOK)
					{
						if(cTimer.bPrintNextMenuItem == true)	
						{
							PrintToDisplay((char*)"|01->00| SYSTEM OK\n  2012-12-08 17:19");
						}
						else
							PrintToDisplay((char*)"|01->00| SYSTEM OK\n<- [BTN4]  [BTN6] ->");
					}					
			}			
		
			
			break;
		case mainMENU2:
			/* #01#->Zone 1 (MK) OK
			#00# to bypass
			--------------------
			--------------------
			01->Zone 1 offline
			#00# to plug it in*/
			if(Menu.DisplayZoneHelpText == true && Menu.subPos == 0)
			{
				uint8_t tmpKey = 0xFF;
				
				tmpKey = GetKeyPressed();
				tmpKey++;
				if(tmpKey!=0xFF)
					cTimer.bReadKeyPress = false;
				if(tmpKey == 4)
				{
					Menu.subPos = 8;
					Menu.iZoneHelpTextCount = 0;
					Menu.DisplayZoneHelpText = false;
					//_delay_ms(500);
					break;
				}
				else if(tmpKey == 6)
				{
					Menu.subPos = 1;
					Menu.iZoneHelpTextCount = 0;
					Menu.DisplayZoneHelpText = false;
					//_delay_ms(500);
					break;
				}
				if(cTimer.bPrintNextMenuItem == true)	//cTimer controls this variable.
				{
					PrintToDisplay((char*)"|02->00|ZONE STATUS\n  2012-12-08 17:19");
				}
				else
					PrintToDisplay((char*)"|02->00|ZONE STATUS\n<- [BTN4]  [BTN6] ->");
						
									
			}
			else
			{
				if((ScanSections(Menu.subPos) & stateOK) == stateOK)
				{	
					PrintToDisplay("|0%d->0%d|ZONE STATUS\n     Zone %d OK",Menu.mainPos, Menu.subPos, Menu.subPos);
				}
				else if((ScanSections(Menu.subPos) & stateALARM) == stateALARM)
				{
					PrintToDisplay("|0%d->0%d|ZONE STATUS\n    Zone %d ALARM",Menu.mainPos, Menu.subPos, Menu.subPos);
				}
				else if((ScanSections(Menu.subPos) & stateSABOTAGE) == stateSABOTAGE)
				{	PrintToDisplay("|0%d->0%d|ZONE STATUS\n  Zone %d SABOTAGE",Menu.mainPos, Menu.subPos, Menu.subPos);
				} 
			}				
			
				
			break;
		case mainMENU3:
			//PrintToDisplay("|03->00| SETUP\nDate and time")
			//_delay_ms(1000);
			
			if(Menu.bSettingClock==false)
				{
					//strcpy(Brain.lcdRow2, "yyyy-mm-dd hh-mm");
					//PrintToDisplay("|03->00| SETUP\n%s", Brain.lcdRow2);
					_delay_ms(50);
					cTimer.countcTimerDisplayLoginScreen = 120;
					cTimer.bDisplayLoginText = false;
					Menu.bSettingClock = true;
					break;
				}
			else
				{
					if(cTimer.bDisplayLoginText)
						Menu.bSettingClock=true;
				}
			
			tmpKey=0xFF;
			tmpKey = GetKeyPressed();

			if(tmpKey != 0xFF)
			{
				cTimer.bReadKeyPress = false;
				Menu.bSettingClock=true;
/*
				if((Menu.clockPos == CLOCKOFFSET + 4) || 
					(Menu.clockPos == CLOCKOFFSET + 7) || 
					(Menu.clockPos == CLOCKOFFSET + 13) )
				{
						Brain.lcdRow2[Menu.clockPos + CLOCKOFFSET] = 0x2D;
						Menu.clockPos++;
				}	
				if(Menu.clockPos == CLOCKOFFSET + 10)
				{
						Brain.lcdRow2[Menu.clockPos + CLOCKOFFSET] = 0x20;
						Menu.clockPos++;
				}
				if(Menu.clockPos > CLOCKOFFSET + 15)
				{
					_delay_ms(100);
					for(int i = 0; i < 4; i++)
					{
						Brain.Year[i]=Brain.lcdRow2[0+CLOCKOFFSET];
					}
					for(int c = 0; c < 2; c++)
					{
						Brain.Month[c]=Brain.lcdRow2[5+CLOCKOFFSET];
						Brain.Day[c]=Brain.lcdRow2[8+CLOCKOFFSET];
						Brain.Hour[c]=Brain.lcdRow2[11+CLOCKOFFSET];
						Brain.Minute[c]=Brain.lcdRow2[14+CLOCKOFFSET];
						Menu.clockPos = 0;
					}
				}
				Brain.lcdRow2[Menu.clockPos] = tmpKey + 49;
				Menu.clockPos ++;
				
				PrintToDisplay("|03->00| SETUP\n%s", Brain.lcdRow2);*/
			}
		/*
								 Menu #03# Date/cTimer
								 --------------------
								 #03# Date & cTimer:
								  yyyy-mm-dd hh-mm
						
								 --------------------*/
	/*
					if(Menu.subPos==1)
					{
						UpdateDisplay("Enter date & cTimer", "on the next page", printDELAY0);
					}
					else if(Menu.subPos>1 || Menu.subPos<1)
					{
						UpdateDisplay("Enter date & cTimer", "on the next page", printDELAY0);
						_delay_ms(500);
						lcd_clrscr();

						int Pos=0;
						Key=0xFF;
						bool boooolan = false;
						while(boooolan==false)
						{
							_delay_ms(200);
							Key = GetCharPressed();
							if(Key==0xFF)
							{
								continue;
								_delay_ms(10);
							}
							Menu.lcdRow1[Pos] = Key + 49;
							lcd_putc(Menu.lcdRow1[Pos]);
							Pos++;
	
							if(strlen(Menu.lcdRow1)>10)
								boooolan=true;
							
						}
						
					}
					
*/
	
			break;
		case mainMENU4:
			break;
		case mainMENU5:
			break;
		case mainMENU6:
			break;
		case mainMENU7:
			break;
		case mainMENU8:
			if(Brain.bLoginState)
			{
				strcpy(Brain.lcdRow2, "[*]-DEL   [#]RETURN");

				for(int i=0; i<Brain.countLoginPass; i++)
				{
					if(Brain.enteredPassword[0+i] == 0x30)
						TempLine[PASSWORDOFFSET+i] = 0x30;
					else
						TempLine[PASSWORDOFFSET+i] = Brain.enteredPassword[0+i];
					TempLine[PASSWORDOFFSET+1+i] = 0;
				}
				if(Brain.countLoginPass < 1)
				{
					TempLine[PASSWORDOFFSET] = '\0';
				}		
	
				UpdateDisplay(TempLine, Brain.lcdRow2, printDELAY0);		
		
				tempPass = 0xFF;
				tempPass = GetKeyPressed();
	
		
				if(tempPass!=0xFF)
				{

					cTimer.bReadKeyPress = false;
					tempPass++;
		
					if(tempPass == 10)
					{	
						/* DELETE */
						Brain.enteredPassword[Brain.countLoginPass] = '\0';
						Brain.countLoginPass--;
						if(Brain.countLoginPass < 1)
							Brain.countLoginPass = 0;
						Brain.enteredPassword[Brain.countLoginPass] = '\0';
						_delay_ms(100);
				
					}
					else if(tempPass == 12)
					{	
				
						/* RETURN */
						if(strcmp(Brain.enteredPassword, Brain.correctPassword) == 0)
						{	
					
					
							Brain.countLoginPass=0;
							Brain.bLoginState=false;
							Brain.enteredPassword[0] = '\0';
							PrintToDisplay("Correct Password\nLogging in...");
							Menu.mainPos = mainMENU1;
							Menu.subPos = 0;
						}
						else
						{
							Brain.countLoginPass = 0;
							Brain.bLoginState = false;
							Brain.enteredPassword[0] = '\0';
							Brain.enteredPassword[1] = '\0';
							Brain.enteredPassword[2] = '\0';
							Brain.enteredPassword[3] = '\0';
							Brain.enteredPassword[4] = '\0';
							Brain.enteredPassword[5] = '\0';
							PrintToDisplay("Wrong Password\nSorry!");
							Menu.subPos = 0;
							Menu.mainPos = mainMENU1;
						}
				
					}
					else
					{	/* 1-9	*/
						Brain.enteredPassword[Brain.countLoginPass] = tempPass + 48;
						Brain.countLoginPass++;
					}
			
				}
			}									
			break;
		
		
		default:
			break;
	}
	
return 0;
}

void MenuNavigation()
{
	uint8_t Key=0xFF;

	Key=0xFF;
	Key = GetKeyPressed();
	if(Key != 0xFF)
		Key++;
	if(Key != 0xFF)
	{
		cTimer.bAutoLogOut = false;

		cTimer.bReadKeyPress = false;
		if(Key==8)
		{	Menu.subPos = 0;
			Menu.mainPos++;
			if(Menu.mainPos>mainmenuITEMS)
				Menu.mainPos=1;
		}
		else if(Key==2)
		{	
			Menu.subPos = 0;
			Menu.mainPos--;
			if(Menu.mainPos<1)
				Menu.mainPos=mainmenuITEMS;
		}
		else if (Key==4)
		{	
			Menu.subPos--;
			if(Menu.subPos<1)
				Menu.subPos=submenuITEMS;

		}
		else if (Key==6)
		{
			Menu.subPos++;
			if(Menu.subPos>submenuITEMS)
				Menu.subPos=1;
		}
		if(Key != 4 && Key != 6)
		{
			Menu.subPos = 0;
			Menu.DisplayZoneHelpText = true; //Display help sub menu text menu 2	
		}			
	}	
}
void CheckForLogIn()
{	

}


uint8_t GetKeyPressed()
{
	uint8_t rtn = 0xFF;
	uint8_t r = 0;
	uint8_t c = 0;

	if(cTimer.bReadKeyPress == false)
	{
		
		return 0xFF;
	} else
	{
		cTimer.countcTimerReadKeyPress = 0;
		KEYPAD_PORT|= 0X0F; //0000 1111

		r = 0;
		c = 0;
		//rtn = 0xFF;
		_delay_ms(2);	
		for(c=0;c<3;c++)
		{
			KEYPAD_DDR &= ~(0X7F); //0111 1111 Set all DDR to outputs
			//KEYPAD_DDR = 0xFF;
			//Toggle colums, to input, one by one.
			KEYPAD_DDR|=(0X40>>c); //0100 0000
			_delay_ms(2);
			for(r=0;r<4;r++)
			{
				//If keypadpin in col c and row r is low, then
				if(!(KEYPAD_PIN & (0X08>>r))) //0000 1000
				{
					_delay_ms(2);
					//return row cTimers 3 + column. example. row=0, col=0;
					//
					//rtn = (r*3+c);
					return (r*3+c);

					/*if(rtn == 0xFF)
					{
						return rtn;
					}
					if(cTimer.countcTimerReadKeyPress > 5 || cTimer.bReadKeyPress == true)
					{
						cTimer.bReadKeyPress = false;
						cTimer.countcTimerReadKeyPress = 0;

						return rtn;
					}*/
				

				}
			}
		}
	}	

	return 0XFF;//Indicate No key pressed
}

void PrintFancyMessage(char *string1, char *string2, int len)
{
	int iCursor = len/2;
	
	for(int n = 0; n < iCursor; n++)
	lcd_command(LCD_MOVE_CURSOR_LEFT);
	lcd_puts(string1);
	lcd_putc('\n');
	for(int n = 0; n < iCursor; n++)
	lcd_command(LCD_MOVE_CURSOR_LEFT);
	lcd_puts(string2);

	for(int i = 0; i < len; i ++)
	{
		lcd_command(LCD_MOVE_DISP_RIGHT);
		_delay_ms(400);
	}
	lcd_clrscr();
}

unsigned long hash(unsigned char *str)
{
	unsigned long hash = 5381;
	int c;
	
	while (c = *str++)
	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	
	return hash;
}


void PrintToDisplay(const char *fmt, ...)
{	
	char tempDisplay[45];
	//if(strcmp(oldTempDispl, tempDisplay) == 0)
	//{
	//	return;
	//}

	//if(Brain.uidDisplay == Brain.olduidDisplay)

int ret = 0;
	/*int centerCount1 = 0;
	int centerCount2 = 0;
	int centerCountSize1 = 0;
	int centerCountSize2 = 0;*/
	memset(&tempDisplay[0], 0, 41);
	//memset(&Brain.lcdRow1[0], 0, 40);
	//memset(&Brain.lcdRow2[0], 0, 40);

	va_list arg;
	char row1[25];
	char row2[25];
	va_start(arg, fmt);
	ret = vsprintf(tempDisplay, fmt, arg);
	va_end(arg);
	

	Brain.uidDisplay = hash((unsigned char*)tempDisplay);
	
	char * pch;
	char token[2];
	int test;
	strcpy(token, "\n");
	pch = strtok (tempDisplay,token);
	if(pch == NULL)
		return;
	else if(strlen(Brain.oldlcdRow1) != strlen(row1) && (pch != NULL))
		strcpy(row1, pch);
	else
		return;
	
	while(pch != NULL) //Split the string into two lines, Brain.lcdRow1 & 2
	{
		pch = NULL;
		pch = strtok(NULL, token);
		test = strlen(pch);
		if(test > 0)
		{
			strcpy(row2, pch);
			break;
		}
		if(cTimer.bPrintTimeout)
		break;//safety feature
	}
	Brain.olduidDisplay = Brain.uidDisplay;
	//strcpy(oldTempDispl, tempDisplay);
	if(strcmp(Brain.oldlcdRow1, row1)==0 && strcmp(Brain.oldlcdRow2, row2) == 0)
	{

		return;
	}
	else
	{	
		strcpy(Brain.oldlcdRow1, row1);
		strcpy(Brain.oldlcdRow2, row2);
		lcd_clrscr();
		lcd_puts(row1);
		lcd_putc('\n');
		lcd_puts(row2);
		//strcpy(Brain.lcdRow1, Brain.oldlcdRow1);
		//strcpy(Brain.lcdRow2, Brain.oldlcdRow2);
		
	}	
	_delay_ms(10);

}

void UpdateDisplay(char *firstrow, char *secondrow, char printdelay)
{

	if(strcmp(Brain.oldlcdRow1, firstrow)==0 &&
	strcmp(Brain.oldlcdRow2, secondrow)==0)
	{
		return;
	}
	else
	{
		lcd_clrscr();
		lcd_puts(firstrow);
		lcd_putc('\n');
		lcd_puts(secondrow);

		switch(printdelay)
		{
			case printDELAY0:
				_delay_us(5);
			break;
			
			case printDELAY10:
			_delay_ms(10);
			break;
			
			case printDELAY100:
			_delay_ms(100);
			break;
			
			case printDELAY300:
			_delay_ms(300);
			break;
			
			case printDELAY500:
			_delay_ms(500);
			break;
			
			case printDELAY1000:
			_delay_ms(1000);
			break;
			
			case printDELAY2000:
			_delay_ms(2000);
			break;

			
			default:

			break;
		}
	}
	return;
}