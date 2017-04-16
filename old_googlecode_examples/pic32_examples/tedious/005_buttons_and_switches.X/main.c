// File:   main.c
// Author: Yuri Panchul

#include <p32xxxx.h>

typedef unsigned uint;

void main(void)
{
    // AD1PCFG = ~0;

    TRISE = 0;
    PORTE = 0;

    for (;;)
    {
        PORTEbits.RE0 = PORTFbits.RF1;
        PORTEbits.RE1 = PORTDbits.RD5;
        PORTEbits.RE2 = PORTDbits.RD6;
        PORTEbits.RE3 = PORTDbits.RD7;

        PORTEbits.RE4 = PORTDbits.RD8;
        PORTEbits.RE5 = PORTDbits.RD9;
        PORTEbits.RE6 = PORTDbits.RD10;
        PORTEbits.RE7 = PORTDbits.RD11;
    }
}
