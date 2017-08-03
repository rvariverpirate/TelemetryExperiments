#include "NU32.h"
#include "delays.h"
#include "nrf24l01.h"
#include <stdio.h>
// Connections
// Baud Rate = 230400
// SDO4 -> SDI3 (pin F5 -> pin D2)
// SDI4 -> SDO3 (pin F4 -> pin D3)
// SCK4 -> SCK3 (pin B14 -> pin D1)
void InitializeIO(void);
//void checkForUARTData(void);
void ToggleLED(void);

int main(void){
	char buf[100] = {};
	unsigned char data;// byte of data sent/received
	unsigned char dataArray;// UART read needs array
	unsigned int count;// counter for for loop

	// Configuure the PIC 32 and initialize the NRF
	NU32_Startup();// setup UART
	setup_spi4();// setup SPI4 as master
	InitializeIO();// setup GPIO pins
	nrf24l01_initialize_debug(false, 1, false); //initialize the 24L01 to the debug configuration as TX, 1 data byte, and auto-ack disabled
	
	NU32_WriteUART3("Running...\r\n");  
	while(1)
	{
		//CheckErrorsUSART()
		//check UART status register to see if data has been received.  if so, process
		while(U3STAbits.URXDA)
		{

			data = NU32_ReadCharUART3(); //get data from UART
			nrf24l01_write_tx_payload(&data, 1, true); //transmit received char over RF
			NU32_WriteUART3("You entered: ");
			NU32_WriteCharUART3(data);
			NU32_WriteUART3("\n");
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
					nrf24l01_read_rx_payload(&data, 1); //get the payload into data
					DelayUS(130); //wait for receiver to come from standby to RX
					break;
				}
				
				//if loop is on its last iteration, assume packet has been lost.
				if(count == 19999)
					data = '?';
			}
			
			nrf24l01_irq_clear_all(); //clear interrupts again
			//printf("%c", data); //print the received data (or ? if none) to the screen
			//sprintf(buf, "Received 0x%04x", data);
			//NU32_WriteUART3(buf);
			NU32_WriteUART3("You received: ");
			NU32_WriteUART3(&data);
			NU32_WriteUART3("\n");
			DelayUS(130); //wait for receiver to come from standby to RX
			nrf24l01_set_as_tx(); //resume normal operation as a TX
											
			ToggleLED(); //toggle the on-board LED as visual indication that the loop has completed
		}
	}
}

void InitializeIO(void)
{
	AD1CON1 = 0x00; //disable AD converter functionality on PORTE
	TRISEbits.TRISE0 = 0; //make PORTA.0 an output to control LED
	PORTEbits.RE0 = 1; //turn on LED
	
	TRISBbits.TRISB1 = 1; //make sure that PORTB.1 is input since it is IRQ pin
}

//toggles on-board LED
void ToggleLED(void)
{
	PORTEbits.RE0 = ~PORTEbits.RE0; //invert the bit that controls the LED
}