#include "NU32.h"
void setup_spi4(void){

	// Master - SPI4 pins are: SDI4(F4), SDO4(F5), SCK4(B14), SS4(B8; not connected)
	// since the PIC is just starting, we know that SPI is off. We rely on defaults here
	SPI4BUF;// clear the RX buffer by reading from it
	SPI4BRG = 0x4;// baud rate to 8 MHz [SPI4BRG = (8000000/(2*desired)) - 1]
	SPI4STATbits.SPIROV = 0;// clear the overflow bit
	SPI4CONbits.MODE32 = 0;// use 8 bit mode
	SPI4CONbits.MODE16 = 0;
	SPI4CONbits.MSTEN = 1;// master operation
	SPI4CONbits.ON = 1;// turn on spi 4

}
void spi4_write(unsigned char byte){
	SPI4BUF = byte;// write single byte to SPI4
}
unsigned char spi4_read(void){
	while(!SPI4STATbits.SPIRBF){
			;// wait until data is ready
		}
	return SPI4BUF;// return the byte
}
unsigned char spi4_send_read_byte(unsigned char byte){
	SPI4BUF = byte;// write single byte
	while(!SPI4STATbits.SPIRBF){
		;// wait until data is ready
	}
	return SPI4BUF;//SPI4BUF;// return the byte
}
