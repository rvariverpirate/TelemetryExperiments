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
	//main program loop 
   while(1) 
   { 
    //wait until a packet has been received 
    while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active())); 
       
    nrf24l01_read_rx_payload(&data, 1); //read the packet into data 
    nrf24l01_irq_clear_all(); //clear all interrupts in the 24L01 
       
    DelayUS(130); //wait for the other 24L01 to come from standby to RX 
       
    nrf24l01_set_as_tx(); //change the device to a TX to send back from the other 24L01 
    nrf24l01_write_tx_payload(&data, 1, true); //transmit received char over RF 
       
    //wait until the packet has been sent 
    while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_tx_ds_active())); 

    nrf24l01_irq_clear_all(); //clear interrupts again 
    nrf24l01_set_as_rx(true); //resume normal operation as an RX 

    NU32_WriteUART3("Receiver received: ");
	NU32_WriteUART3(&data);
	NU32_WriteUART3("\n");
	DelayUS(130); //wait for receiver to come from standby to RX

    ToggleLED(); //toggle the on-board LED as visual indication that the loop has completed 
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