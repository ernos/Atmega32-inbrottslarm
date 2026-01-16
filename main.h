/*
 * main.h
 *
 * Created: 2012-03-29 21:32:15
 *  Author: Max
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/* Print delays. _delay_ms needs a constant. */
#define printDELAY0		0x00
#define printDELAY10	0x01
#define printDELAY100	0x02
#define printDELAY300	0x03
#define printDELAY500	0x04
#define printDELAY1000	0x05
#define printDELAY2000	0x06
#define printDELAY5000	0x07

/* Different states - run modes for program. activated, alarm, login etc */
#define stateOK				1
#define stateALARM			2
#define stateSABOTAGE		4
#define stateDEACTIVE		8
#define stateLOGIN			16
#define stateCONFIGURE		32
#define stateDISCONNECTED	64

/* MENU - menuOVERSIKT menuLARMER menuSABOTAGE */
#define menuOVERSIKT	0
#define menuLARMER		1
#define	menuSABOTAGE	2

/* Defines which port the keypad uses */
#define KEYPAD_PORT PORTB
#define KEYPAD_DDR   DDRB
#define KEYPAD_PIN   PINB

int main(void);
//ISR(INT0_vect);
void Setup(void);
void SetLedStatus();

char ScanSections(int Section); //10 to scan all sections, or choose a section number, returns sections state.
void Meny();
extern void UpdateDisplay(char *firstrow, char *secondrow, char printdelay);

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


//Programming
#define EEPROM_SEKTIONS_USED_ADDR	0


/* For communicating alarms between the "brain" and the visual menu 
	This way I can do a bitflag check from the brain instead of looping through an array
	*/
int AlarmingSections(char section, char type);
#define SECTION1	1
#define SECTION2	2
#define SECTION3	4
#define SECTION4	8
#define SECTION5	16
#define SECTION6	32
#define SECTION7	64
#define SECTION8	128

struct sComputer{
	char lcdRow1[40];
	char oldlcdRow1[40];
	char lcdRow2[40];
	char oldlcdRow2[40];
	char enteredPassword[5], correctPassword[5];
	char Year[4], Month[2], Day[2], Hour[2], Minute[2];
	char state;
	char alarmingSections;
	
	bool bLoginState;
	bool bLoggedIn;
	
	unsigned long uidDisplay, olduidDisplay; //unique number for every new screen
	
	int countLoginPass;
	int countAlarm;
	int countSabotage;
	int countOK;
};

struct structTimer{	/* Helps us pause in cTimer without slowing down CPU progress using built in cTimer with interrupts. */
	int countcTimerFlipMenuText;
	int countcTimerReadKeyPress;
	int countcTimerDisplayLoginScreen;
	int countcTimerAutoLogOut;
	int countcTimerPrintTimeout;
	bool bPrintTimeout;
	bool bAutoLogOut;
	bool bPrintNextMenuItem; //Toggles to show next cTimer, example on first menu screen. Date / cTimer and buttons to scroll sideways
	bool bReadKeyPress;
	bool bDisplayLoginText;
	
};

#endif /* MAIN_H_ */

/*
ACME	ADEN	MUX2..0   Analog Comparator Negative Input
0		x		xxx			AIN1
1		1		xxx			AIN1
1		0		000			ADC0
1		0		001			ADC1
1		0		010			ADC2
1		0		011			ADC3
1		0		100			ADC4
1		0		101			ADC5
1		0		110			ADC6
1		0		111			ADC7

*/