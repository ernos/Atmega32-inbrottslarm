/*
 * comm.cpp
 *
 * Created: 2012-12-22 13:52:54
 *  Author: Max
 */ 
/*


#include <avr/io.h>
#include <stdio.h>

#include "comm.h"

cCommunication Comm;

void cCommunication::InitUsart( unsigned int ubrr)
{
	#ifdef _STDIO_H_
	
		mystdout = FDEV_SETUP_STREAM(usart_transmit, NULL, _FDEV_SETUP_WRITE);
		stdout = &mystdout;
		
	#endif

	//stdout = &mystdout;
	// Set baud rate 
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)(ubrr);
	//USART Double speed
	UCSRA = (1<<U2X);
	// Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN)
	#if _USE_USART_INTERRUPT
	|(1<<UDRIE)|(1<<RXCIE);
	#else
	|(1<<RXCIE);
	#endif
	// Set frame format: 8data, 1 stop bit
	UCSRC = (1<<URSEL)|(3<<UCSZ0);

	//Set PORTD.RXD As input ,enable pull-up
	PORTD |=(1<<0);
	DDRD |=(1<<1); //TXD is output
}
*/