#include <avr/io.h>
#include "spi.h"

void SPI_Master_Init(void)
{
	/* Set MOSI and SCK output, all others input */
	DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
	/* Start Transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
		;
}

unsigned char SPI_SlaveReceive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)))
		;
	/* Return Data Register */
	return SPDR;
}

unsigned char spi_send_read_byte(unsigned char byte){
	SPDR = byte;
	while(!(SPSR & (1<<SPIF))){
		return SPDR;
	}
}