#ifndef _SPI4_H
#define _SPI4_H

void setup_spi4(void);
void spi4_write(unsigned char byte);
unsigned char spi4_read(void);
unsigned char spi4_send_read_byte(unsigned char byte);

#endif