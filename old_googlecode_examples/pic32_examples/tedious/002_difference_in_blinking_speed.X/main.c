//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm volatile ("nop");
}

void blink (void)
{
    int i;

    TRISE = 0;

    PORTE = 1;

    for (i = 0; i < 8; i++)
    {
        delay (1);
        PORTE <<= 1;
    }
}

void main (void)
{
    int config;

    for (;;)
    {
        // Fast

        CHECON    = 2;
        BMXCONCLR = 0x40;
        CHECONSET = 0x30;

        asm volatile ("mfc0 %0, $16" : "=r" (config));
        config |= 3;
        asm volatile ("mtc0 %0, $16" : "=r" (config));

        blink ();

        // Slower

        CHECON    = 2;
        BMXCONCLR = 0x40;
        CHECONSET = 0x30;

        asm volatile ("mfc0 %0, $16" : "=r" (config));
        config &= ~3;
        asm volatile ("mtc0 %0, $16" : "=r" (config));

        blink ();

        // Even more slover

        CHECON    = 7;
        BMXCONCLR = 0x40;
        CHECONCLR = 0x30;

        asm volatile ("mfc0 %0, $16" : "=r" (config));
        config |= 3;
        asm volatile ("mtc0 %0, $16" : "=r" (config));

        blink ();

        // Slow

        CHECON    = 7;
        BMXCONCLR = 0x40;
        CHECONCLR = 0x30;

        asm volatile ("mfc0 %0, $16" : "=r" (config));
        config &= ~3;
        asm volatile ("mtc0 %0, $16" : "=r" (config));

        blink ();
    }
}
