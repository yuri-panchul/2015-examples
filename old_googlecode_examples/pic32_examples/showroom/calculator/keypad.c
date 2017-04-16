//  File:   keypad.c
//  Author: Yuri Panchul
//
//  This code uses fragments adopted from
//  Digital Design and Computer Architecture, Second Edition
//  by David Harris & Sarah Harris

//
//  All pins in this code are hardcoded since it is intended to be
//  a very small easy to read example that is a part of a small program
//  that illustrates the features of a microcontroller.
//
//  This code is neither a part of a large industrial project
//  nor an example how to write portable reusable code.
//  Therefore: less text = easier to read and to follow.
//

#include <p32xxxx.h>

#include "config.h"
#include "keypad.h"

#define n_rows 4
#define n_cols 4

static bool matrix [n_rows][n_cols];

static uint translation [n_rows][n_cols] =
{
    {  1,   2,   3, 0xA,  },
    {  4,   5,   6, 0xB,  },
    {  7,   8,   9, 0xC,  },
    {  0, 0xF, 0xE, 0xD   }
};

#define buf_size 1024

static char buf [buf_size];
static int i_put, i_get;

static bool use_timer_interrupt;

static bool keypad_put (char a)
{
    int next_i_put = i_put + 1;

    if (next_i_put == buf_size)
        next_i_put = 0;

    // Checking buffer overflow

    if (next_i_put == i_get)
        return false;

    buf [i_put] = a;
    i_put = next_i_put;

    return true;
}

static void keypad_poll ()
{
    int row, col;
    int in;

    for (col = 0; col < n_cols; col ++)
    {
        PORTE = ~ (8 >> ((col + 1) & 3));
        in = PORTE >> 4;

        for (row = 0; row < n_rows; row ++)
        {
            bool on = ! (in & 8);
            in <<= 1;

            if (on && ! matrix [row][col])
                keypad_put (translation [row][col]);

            matrix [row][col] = on;
        }
    }
}

//
//  The Timer 1 interrupt is Vector 4, using enable bit IECO<4>
//  and flag bit IFSO<4>, priority IPC1<4:2>, subpriority IPC1<1:0>

void __attribute__ ((interrupt (IPL7))) __attribute__ ((vector (4))) keypad_timer (void)
{
    keypad_poll ();
    IFS0bits.T1IF = 0;
}

void keypad_init (bool use_interrupts)
{
    // init buffers

    memset (matrix, 0, sizeof (matrix));
    i_put = i_get = 0;

    // init keypad port

    TRISE = 0xF0;

    // init interrupt

    use_timer_interrupt = use_interrupts;

    if (! use_timer_interrupt)
        return;

    T1CONbits.ON     = 0;      // turn timer off
    TMR1             = 0;      // reset timer to 0

    T1CONbits.TCKPS  = 3;      // 1:256 prescale
    PR1              = PBCLK_FREQUENCY / 256 / 50;  // 1/50th of a second

    INTCONbits.MVEC  = 1;      // enable multi-vector mode
    IPC1bits.T1IP    = 7;      // interrupt priority
    IPC1bits.T1IS    = 3;      // interrupt subpriority
    IFS0bits.T1IF    = 0;      // clear the Timer 1 interrupt flag
    IEC0bits.T1IE    = 1;      // enable the Timer 1 interrupt

    asm volatile ("ei");       // enable interrupts

    T1CONbits.ON     = 1;      // turn timer on
}

bool keypad_try_get (char * pa)
{
    if (i_get == i_put)
        return false;

    * pa = buf [i_get ++];

    if (i_get == buf_size)
        i_get = 0;

    return true;
}

char keypad_get (void)
{
    char a;

    while (! keypad_try_get (& a))
        if (! use_timer_interrupt)
            keypad_poll ();

    return a;
}
