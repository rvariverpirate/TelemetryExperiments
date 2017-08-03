// USART Section
// Setup for USART
#define FSOC 8000000UL
#define F_CPU 8000000UL
#define BAUD 9600
#define MYUBRR FSOC/16/BAUD-1

// Import libraries
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
// Functions Used for USART Communication
void USART_Init(unsigned int ubrr)
{
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    /*Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    //UCSR0C = (1<<UCSZ00) | (1<<USCZ01);
    UCSR0C = (3<<UCSZ00);
}

void USART_Transmit(unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;

}


unsigned char USART_Receive( void )
{
    /* Wait for data to be received */
    while ( !(UCSR0A & (1<<RXC0)) );
    /* Get and return received data from buffer */
    return UDR0;
}

// Loops though given string and sends each char to serial port
void USART_println(unsigned char *myString)
{
	int i = 0;//
	while(myString[i] != '\0')
	{
		USART_Transmit(myString[i]);
		_delay_us(10);
		i ++;
	}
	USART_Transmit('\n');// Line Break

}

// This section is used for impleenting printf
int usart_putchar_printf(char var, FILE *stream) {
    if (var == '\n') USART_Transmit('\r');
    USART_Transmit(var);
    return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE);
