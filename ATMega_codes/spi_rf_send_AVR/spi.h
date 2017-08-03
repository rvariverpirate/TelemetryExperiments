#ifndef _SPI_H
#define _SPI_H
#include <avr/io.h>
// Define Ports and pinouts
#define DDR_SPI PORTB// *
#define DD_MOSI 3// *
#define DD_SCK 5// *
//#define SPDR ?// *

void SPI_Master_Init(void);
void SPI_MasterTransmit(char cData);
unsigned char SPI_SlaveReceive(void);
unsigned char spi_send_read_byte(unsigned char byte);

#endif