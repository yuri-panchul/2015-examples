#include <p32xxxx.h>
#include <plib.h>

#define SYSCLK_FREQUENCY  (40 * 1000 * 1000)
#define PBCLK_FREQUENCY   (10 * 1000 * 1000)

//
//  The Timer 1 interrupt is Vector 4, using enable bit IECO<4>
//  and flag bit IFSO<4>, priority IPC1<4:2>, subpriority IPC1<1:0>

int led_value = 0;

void __attribute__ ((interrupt (IPL7))) __attribute__ ((vector (4))) keypad_timer (void)
{
    PORTBbits.RB12 = led_value;
    led_value = ! led_value;

    IFS0bits.T1IF = 0;
}

void main (void)
{
    // Optimize performance

    SYSTEMConfigPerformance (SYSCLK_FREQUENCY);

    // Init output

    TRISBbits.TRISB12 = 0;

    // Setup timer interrupt

    T1CONbits.ON     = 0;      // turn timer off
    TMR1             = 0;      // reset timer to 0

    T1CONbits.TCKPS  = 3;      // 1:256 prescale
    PR1              = PBCLK_FREQUENCY / 256 / 1;  // 1/50th of a second

    INTCONbits.MVEC  = 1;      // enable multi-vector mode
    IPC1bits.T1IP    = 7;      // interrupt priority
    IPC1bits.T1IS    = 3;      // interrupt subpriority
    IFS0bits.T1IF    = 0;      // clear the Timer 1 interrupt flag
    IEC0bits.T1IE    = 1;      // enable the Timer 1 interrupt

    asm volatile ("ei");       // enable interrupts

    T1CONbits.ON     = 1;      // turn timer on

    // Eternal loop

    for (;;)
        ;
}
