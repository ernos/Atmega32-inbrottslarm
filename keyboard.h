/*
 * keyboard.h
 *
 *   1 start bit.  This is always 0.
 *   8 data bits, least significant bit first.
 *   1 parity bit (odd parity).
 *   1 stop bit.  This is always 1.
 *   1 acknowledge bit (host-to-device communication only)
 *
 * Created: 2012-03-29 22:05:17
 *  Author: Max
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

int pkCurrentBit = 0;
int pkStart = 0;
int pkParity = 0;
int pkStop = 0;
int pkAck = 0;
char pkData;


#endif /* KEYBOARD_H_ */