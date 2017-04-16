//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>
#include <plib.h>

#include "types.h"
#include "config.h"
#include "uart.h"

void running_fast ()
{
    uint config;
    
    CHECON    = 2;
    BMXCONCLR = 0x40;
    CHECONSET = 0x30;

    asm volatile ("mfc0 %0, $16" : "=r" (config));
    config |= 3;
    asm volatile ("mtc0 %0, $16" : "=r" (config));
}

uint count;

void interrupt_init (void)
{
    PR1   = 32768 - 1;  // Toggle every half second
    T1CON = 0x8002;     // enabled, prescaler 1:1, use secondary oscillator

    mT1SetIntPriority (1);
    mT1ClearIntFlag ();
    INTEnableSystemSingleVectoredInt ();
    mT1IntEnable (1);
}

#pragma interrupt interrupt_handler ipl1 vector 0

void interrupt_handler (void)
{
    uart_putn   (count);
    uart_put_nl ();

    mT1ClearIntFlag ();
}

void main (void)
{
//    running_fast ();
/*
    uart_init (PBCLK_FREQUENCY, 9600);

    uart_puts   ("Frequency: ");
    uart_putn   (PBCLK_FREQUENCY);
    uart_put_nl ();

    interrupt_init ();
*/
    TRISE = 0;

    for (count = 0;; count++)
        PORTE = count >> 8;

        if ((count & 0xFFFF) == 0)
            PORTE = count >> 16;
}
