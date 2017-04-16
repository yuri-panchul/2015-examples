//  File:   delay.c
//  Author: Yuri Panchul
//
//  This code uses fragments adopted from
//  Digital Design and Computer Architecture, Second Edition
//  by David Harris & Sarah Harris

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"

void delay_for_1000_nops_x (uint n)
{
    while (n --)
    {
        int i;

        for (i = 0; i < 1000; i++)
            asm volatile ("nop");
    }
}

void delay_for_1000_nops (void)
{
    delay_for_1000_nops_x (1);
}

void delay_micros (uint n)
{
    const uint function_overhead = 6;

    while (n > 1000)  // To avoid timer overflow
    {
        delay_micros (1000);
        n -= 1000;
    }

    if (n > function_overhead)
    {
        TMR1 = 0;  // Reset timer to 0

        PR1 
            =   (n - function_overhead)
              * (PBCLK_FREQUENCY / 1000)
              / 1000;

        IFS0bits.T1IF = 0;       // Clear overflow flag

        T1CONbits.ON  = 1;       // Turn timer on

        while (! IFS0bits.T1IF)  // Wait until overflow flag is set
            ;
    }
}

void delay_millis (uint n)
{
    while (n --)
        delay_micros (1000);
}

void delay_seconds (uint n)
{
    while (n --)
        delay_millis (1000);
}
