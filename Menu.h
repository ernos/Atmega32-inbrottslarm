/*
 * Menu.h
 *
 * Created: 2012-12-08 16:32:34
 *  Author: Max
 */ 
#define CLOCKOFFSET 0

#ifndef MENU_H_
#define MENU_H_


/* Alphabet works like a mobile phone:
1 - n/a		2 - abc		3 - def
4 - ghi		5 - jkl		6 - mno
7 - pqrs	8 - tuv		9 - wxyz
* - n/a		0 - [space]	# - Punctuation
*/

/* Main menu's */
#define mainMENU1		1
#define mainMENU2		2
#define mainMENU3		3
#define mainMENU4		4
#define mainMENU5		5
#define mainMENU6		6
#define mainMENU7		7
#define mainMENU8		8
/* sub menu's */
#define subMENU1		1
#define subMENU2		2
#define subMENU3		3
#define subMENU4		4
#define subMENU5		5
#define subMENU6		6
#define subMENU7		7
#define subMENU8		8

#define menuERROR		255

#define mainmenuITEMS	3
#define submenuITEMS	8

struct sMenu{

	signed char mainPos;
	signed char subPos;
	bool DisplayZoneHelpText, bZoneHelpTest; //Display help sub menu text menu 2
	bool bSettingClock;
	int clockPos;
	int iZoneHelpTextCount;
	};

void MenuNavigation();
void PrintFancyMessage(char *string1, char *string2, int len);
uint8_t GetKeyPressed();
void CheckForLogIn();
void DisplayMenu();
char PrintMenu();
void UpdateDisplay(char *firstrow, char *secondrow, char printdelay);
void PrintToDisplay(const char *fmt, ...);

#endif /* MENU_H_ */