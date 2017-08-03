// Setup for SPI
#define CE_PIN 0// PC0
#define CE_PORT PORTC
#define CSN_PIN 2// PB2 (may want to change this)
#define CSN_PORT PORTB// This may cause problems?
#define IRQ_PIN 4 // PD4
#define IRQ_PORT PORTD
#define LED_PIN 2// PORTD

// Includes
#include "nrf24l01.h"
#include "SimpleUART.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

// Connections
// Baud Rate = 9600
// SCK -> SCK (pin PB5 -> pin 5)
// MOSI -> MOSI (pin PB3 -> pin 6)
// MISO -> MISO (pin PB4 -> pin 7)
void InitializeIO(void);
//void checkForUARTData(void);
void ToggleLED(void);

int main(void){
	char buf[100] = {};
	char data;// byte of data sent/received
	unsigned int count;// counter for for loop

	// Configuure the PIC 32 and initialize the NRF
	USART_Init(MYUBRR);// setup UART
	SPI_Master_Init();// setup SPI4 as master
	InitializeIO();// setup GPIO pins
	nrf24l01_initialize_debug(false, 1, false); //initialize the 24L01 to the debug configuration as TX, 1 data byte, and auto-ack disabled
	
	USART_println("Running...\r\n");  
	while(1)
	{
		//CheckErrorsUSART()
		//check UART status register to see if data has been received.  if so, process
		while(!(UCSR0A & (1<<RXC0)))
		{

			data = USART_Receive(); //get data from UART
			nrf24l01_write_tx_payload(&data, 1, true); //transmit received char over RF
			USART_println("You entered...");
			USART_Transmit(data);
			USART_println("");
			//wait until the packet has been sent or the maximum number of retries has been reached
			while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active()));

			nrf24l01_irq_clear_all(); //clear all interrupts in the 24L01
			nrf24l01_set_as_rx(true); //change the device to an RX to get the character back from the other 24L01

			//wait a while to see if we get the data back (change the loop maximum and the lower if
			//  argument (should be loop maximum - 1) to lengthen or shorten this time frame
			for(count = 0; count < 20000; count++)
			{
				//check to see if the data has been received.  if so, get the data and exit the loop.
				//  if the loop is at its last count, assume the packet has been lost and set the data
				//  to go to the UART to "?".  If neither of these is true, keep looping.
				if((nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()))
				{
					nrf24l01_read_rx_payload(data, 1); //get the payload into data
					_delay_us(300); // 130 //wait for receiver to come from standby to RX
					break;
				}
				
				//if loop is on its last iteration, assume packet has been lost.
				if(count == 19999)
					data = '?';
			}
			
			nrf24l01_irq_clear_all(); //clear interrupts again
			//printf("%c", data); //print the received data (or ? if none) to the screen
			//sprintf(buf, "Received 0x%04x", &data);
			//USART_println(buf);
			USART_println("You received...");
			USART_Transmit(data);
			USART_println("");
			USART_println("");
			_delay_us(300); // 130 //wait for receiver to come from standby to RX
			nrf24l01_set_as_tx(); //resume normal operation as a TX
											
			ToggleLED(); //toggle the on-board LED as visual indication that the loop has completed
		}
	}
}

void InitializeIO(void)
{
	//AD1CON1 = 0x00; //disable AD converter functionality on PORTE
	//TRISEbits.TRISE0 = 0; //make PORTA.0 an output to control LED
	//PORTEbits.RE0 = 1; //turn on LED
	//TRISBbits.TRISB0 = 1; //make sure that PORTB.0 is input since it is IRQ pin
	DDRD = ~(1<<IRQ_PIN) | (1<<LED_PIN);// Make LED Pin an output and IRQ pin an input
	// ORing w/ LED_PIN is probably redundant but it is more readable
	PORTD = (1<<LED_PIN);// Turn on LED

}

//toggles on-board LED
void ToggleLED(void)
{
	PORTD = PORTD ^ (1<<LED_PIN); //invert the bit that controls the LED
}