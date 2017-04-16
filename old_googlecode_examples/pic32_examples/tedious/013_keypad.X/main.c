//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "types.h"
#include "config.h"
#include "uart.h"

void main (void)
{
    int row, col;

    uart_init (PBCLK_FREQUENCY, 9600);

    AD1PCFG = ~0;

    TRISBbits.TRISB8  = 0;
    TRISFbits.TRISF5  = 0;
    TRISFbits.TRISF4  = 0;
    TRISBbits.TRISB14 = 0;

    TRISBbits.TRISB0  = 1;
    TRISBbits.TRISB1  = 1;
    TRISDbits.TRISD0  = 1;
    TRISDbits.TRISD1  = 1;

    for (;;)
    for (col = 1; col <= 4; col ++)
    {
        PORTBbits.RB8  = col != 4;
        PORTFbits.RF5  = col != 3;
        PORTFbits.RF4  = col != 2;
        PORTBbits.RB14 = col != 1;

        for (row = 1; row <= 4; row ++)
        {
            bool off;

            switch (row)
            {
                case 4: off = PORTBbits.RB0; break;
                case 3: off = PORTBbits.RB1; break;
                case 2: off = PORTDbits.RD0; break;
                case 1: off = PORTDbits.RD1; break;
            }

            if (! off)
            {
                uart_putn   (col);
                uart_puts   (" ");
                uart_putn   (row);
                uart_put_nl ();
            }
        }
    }
}
