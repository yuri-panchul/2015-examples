//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

typedef unsigned uint;

void uart_init (void)
{
    U1STAbits.UTXEN = 1;   // enable transmit pin
    U1STAbits.URXEN = 1;   // enable receive pin
    U1BRG           = 80 * 1000000 / 16 / 115200 - 1;
    U1MODEbits.ON   = 1;   // enable UART
}

void uart_putc (char c)
{
    while (U1STAbits.UTXBF);  // wait until transmit buffer empty
    U1TXREG = c;              // transmit character over UART
}

void uart_puts (char *s)
{
    while (*s != '\0')
        uart_putc (*s++);

    uart_putc ('\n');
    uart_putc ('\r');
}

void main (void)
{
    int i;
    char buf [32];

    TRISE = 0;

    uart_init ();

    for (;;)
    {
        for (i = 'a'; i <= 'z'; i++)
        {
            PORTE = i;
            uart_putc (i);
        }

        uart_putc ('\r');
        uart_putc ('\n');
        uart_putc ('\r');
        uart_putc ('\n');
    }
}
