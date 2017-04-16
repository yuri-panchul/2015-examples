//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "types.h"
#include "config.h"
#include "uart.h"

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

static uchar buf [buf_size];
static int i_put, i_get;

static bool put (uchar a)
{
    int next_i_put = i_put + 1;

    if (next_i_put == buf_size)
        next_i_put = 0;

    if (next_i_put == i_get)
        return false;

    buf [i_put] = a;
    i_put = next_i_put;

    return true;
}

static void poll ()
{
    int row, col;

    for (col = 0; col < n_cols; col ++)
    {
        PORTBbits.RB8  = col != 3;
        PORTFbits.RF5  = col != 2;
        PORTFbits.RF4  = col != 1;
        PORTBbits.RB14 = col != 0;

        for (row = 0; row < n_rows; row ++)
        {
            bool on;

            switch (row)
            {
                case 3: on = ! PORTBbits.RB0; break;
                case 2: on = ! PORTBbits.RB1; break;
                case 1: on = ! PORTDbits.RD0; break;
                case 0: on = ! PORTDbits.RD1; break;
            }

            if (on && ! matrix [row][col])
                put (translation [row][col]);

            matrix [row][col] = on;
        }
    }
}

void keypad_init ()
{
    memset (matrix, 0, sizeof (matrix));
    i_put = i_get = 0;

    AD1PCFG = ~0;

    TRISBbits.TRISB8  = 0;
    TRISFbits.TRISF5  = 0;
    TRISFbits.TRISF4  = 0;
    TRISBbits.TRISB14 = 0;

    TRISBbits.TRISB0  = 1;
    TRISBbits.TRISB1  = 1;
    TRISDbits.TRISD0  = 1;
    TRISDbits.TRISD1  = 1;
}

bool keypad_try_get (uchar * pa)
{
    if (i_get == i_put)
        return false;

    * pa = buf [i_get ++];

    if (i_get == buf_size)
        i_get = 0;

    return true;
}

uchar keypad_get ()
{
    uchar a;

    while (! keypad_try_get (& a))
        poll ();

    return a;
}


void main (void)
{
    uart_init (PBCLK_FREQUENCY, 9600);
    keypad_init ();

    for (;;)
        uart_put_hex_digit (keypad_get ());
}
