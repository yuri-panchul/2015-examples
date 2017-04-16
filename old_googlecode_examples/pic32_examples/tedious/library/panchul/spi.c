//  File:   spi.c
//  Author: Yuri Panchul
//
//  This code uses fragments adopted from
//  Digital Design and Computer Architecture, Second Edition
//  by David Harris & Sarah Harris

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

void spi_put_new_line (void)
{
    spi_put_char ('\r');
    spi_put_char ('\n');
}

void spi_put_str (char *s)
{
    while (*s != '\0')
        spi_put_char (*s++);
}

void spi_put_dec (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            spi_put_char ('0' + n / i % 10);
    }
}

void spi_put_hex_digit (uint n)
{
    char c;

    c  = n & 0x0f;
    c += c >= 10 ? 'A' - 10 : '0';

    spi_put_char (c);
}

void spi_put_hex_byte (char n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        spi_put_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}

void spi_put_hex (uint n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        spi_put_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}
