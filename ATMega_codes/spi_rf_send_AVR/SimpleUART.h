#ifndef _SIMPLE_UART_H_
#define _SIMPLE_UART_H_

// Setup for USART
#define FSOC 8000000UL
#define F_CPU 8000000UL
#define BAUD 9600
#define MYUBRR FSOC/16/BAUD-1

// Import libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// UART Functions
void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data );
unsigned char USART_Receive( void );
USART_println(unsigned char *myString);
int usart_putchar_printf(char var, FILE *stream);



#endif