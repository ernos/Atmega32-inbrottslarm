#ifndef USART_H
#define USART_H

#include <stdio.h>

int put_uart_char(char c, FILE *fd);

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR 52//FOSC/16/BAUD-1

#define _USE_USART_INTERRUPT 0 // comment this if needed
#define USART_BUFF_SIZE 64
void putstream(unsigned char *src,unsigned int count);
void usart_init( unsigned int );
unsigned char usart_receive( void );
#ifdef _STDIO_H_
//void usart_transmit(char ,FILE *);
//void usart_transmit(char);
#else
void usart_transmit(char);
void puts_h( unsigned char , unsigned long );
void puts(char * );
#endif

#endif