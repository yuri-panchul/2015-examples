//  File:   uart.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "uart.h"

void uart_init (uint hertz, uint baud)
{
    U1STAbits.UTXEN = 1;   // enable transmit pin
    U1STAbits.URXEN = 1;   // enable receive pin
    U1BRG           = hertz / 16 / baud - 1;
    U1MODEbits.ON   = 1;   // enable UART

    uart_put_new_line  ();
    uart_put_str       ("UART: Peripheral bus: ");
    uart_put_dec       (hertz);
    uart_put_str       (" Hz, baud rate: ");
    uart_put_dec       (baud);
    uart_put_new_line  ();
}

void uart_put_char (uchar c)
{
    while (U1STAbits.UTXBF);  // wait until transmit buffer empty
    U1TXREG = c;              // transmit character over UART
}

void uart_put_new_line (void)
{
    uart_put_char ('\r');
    uart_put_char ('\n');
}

void uart_put_str (uchar *s)
{
    while (*s != '\0')
        uart_put_char (*s++);
}

void uart_put_dec (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            uart_put_char ('0' + n / i % 10);
    }
}

void uart_put_hex_digit (uint n)
{
    uchar c;

    c  = n & 0x0f;
    c += c >= 10 ? 'A' - 10 : '0';

    uart_put_char (c);
}

void uart_put_hex (uint n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        uart_put_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}
