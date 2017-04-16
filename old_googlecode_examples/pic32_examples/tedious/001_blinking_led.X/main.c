//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm volatile ("nop");
}

void main (void)
{
    int i;

    for (;;)
    {
        TRISE = 0;

        // Output uses LAT register

        LATE = 0;

        for (i = 0; i < 16; i++)
        {
            delay (1);
            LATE ++;
        }

        LATE = 0;

        for (i = 0; i < 256; i += 16)
        {
            delay (1);
            LATE = i;
        }

        LATE = 0;

        // Output uses PORT register

        PORTE = 1;

        for (i = 0; i < 8; i++)
        {
            delay (1);
            PORTE <<= 1;
        }

        PORTE = 0;

        // Output uses PORTxSET, PORTxCLR and PORTxINV registers

        for (i = 0; i < 4; i++)
        {
            delay (1);
            PORTESET = (1 << i) | (0x80 >> i);
        }

        for (i = 0; i < 4; i++)
        {
            delay (1);
            PORTECLR = (1 << i) | (0x80 >> i);
        }

        delay (1);
        PORTESET = 0xAA;

        for (i = 0; i < 8; i++)
        {
            delay (1);
            PORTEINV = 1 << i;
        }

        PORTE = 0x33;

        for (i = 0; i < 32; i++)
        {
            int temp;

            delay (1);

            temp          = PORTEbits.RE0;
            PORTEbits.RE0 = PORTEbits.RE1;
            PORTEbits.RE1 = PORTEbits.RE2;
            PORTEbits.RE2 = PORTEbits.RE3;
            PORTEbits.RE3 = PORTEbits.RE4;
            PORTEbits.RE4 = PORTEbits.RE5;
            PORTEbits.RE5 = PORTEbits.RE6;
            PORTEbits.RE6 = PORTEbits.RE7;
            PORTEbits.RE7 = temp;
        }

        delay (1); TRISEbits.TRISE0 = 1;
        delay (1); TRISEbits.TRISE1 = 1;
        delay (1); TRISEbits.TRISE2 = 1;
        delay (1); TRISEbits.TRISE3 = 1;
        delay (1); TRISEbits.TRISE4 = 1;
        delay (1); TRISEbits.TRISE5 = 1;
        delay (1); TRISEbits.TRISE6 = 1;
        delay (1); TRISEbits.TRISE7 = 1;
    }
}

