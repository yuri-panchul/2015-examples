//  File:   spi.h
//  Author: Yuri Panchul

#ifndef SPI_H
#define SPI_H

#include "types.h"

void spi_init           (uint baud);

char spi_put_get_char   (char c);

char spi_get_char       (void);

void spi_put_char       (char   c);
void spi_put_new_line   (void    );
void spi_put_str        (char * s);
void spi_put_dec        (uint   n);
void spi_put_hex_digit  (uint   n);
void spi_put_hex_byte   (char   n);
void spi_put_hex        (uint   n);

#endif
