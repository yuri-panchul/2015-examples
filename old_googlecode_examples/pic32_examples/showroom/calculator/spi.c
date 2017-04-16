//  File:   spi.c
//  Author: Yuri Panchul
//
//  This code uses fragments adopted from
//  Digital Design and Computer Architecture, Second Edition
//  by David Harris & Sarah Harris

//
//  All pins in this code are hardcoded since it is intended to be
//  a very small easy to read example that is a part of a small program
//  that illustrates the features of a microcontroller.
//
//  This code is neither a part of a large industrial project
//  nor an example how to write portable reusable code.
//  Therefore: less text = easier to read and to follow.
//

#include <p32xxxx.h>

#include "config.h"
#include "spi.h"

void spi_init (uint baud)
{
    char dummy;

    SPI2CONbits.ON      = 0;        // disable SPI to reset any previous state
    dummy               = SPI2BUF;  // clear receive buffer
    SPI2BRG             = PBCLK_FREQUENCY / 16 / baud - 1;
    SPI2CONbits.MSTEN   = 1;        // enable master mode
    SPI2CONbits.CKE     = 1;        // set clock-to-data timing
    SPI2CONbits.ON      = 1;        // turn SPI on

    // Signal G9 is on the same pin as SPI2SS (Slave Select) signal.
    // Since slave select is not used in this configuration
    // the pin is used as a reset signal for the external SPI slave.

    TRISGbits.TRISG9    = 0;
    PORTGbits.RG9       = 0;
    delay_for_1000_nops_x (1000);
    PORTGbits.RG9       = 1;
    delay_for_1000_nops_x (1000);
}

char spi_put_get_char (char c)
{
    SPI2BUF = c;                   // send data to slave
    while (SPI2STATbits.SPIBUSY);  // wait until SPI transmission complete
    return SPI2BUF;
}

char spi_get_char (void)
{
    return spi_put_get_char (0);
}

void spi_put_char (char c)
{
    (void) spi_put_get_char (c);
}

void spi_put_str (char *s)
{
    while (*s != '\0')
        spi_put_char (*s++);
}
