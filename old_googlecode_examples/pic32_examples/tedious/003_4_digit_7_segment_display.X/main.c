//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm volatile ("nop");
}

/*
 * port  display  segment
 *       pin
 *
 * F4    1        E
 * F6    2        D
 * D7    3        DP
 * D6    4        C
 * D11   5        G
 * D5    6        cathode 4
 *
 * D1    7        B
 * F1    8        cathode 3
 * D0    9        cathode 2
 * D8    10       F
 * F3    11       A
 * F2    12       cathode 1
 *
 */

void setup_display_abcdef_dp ()
{
    TRISDCLR = (  1 << 11) | (  1 << 8) | (   1 << 7) | (  1 << 6) | (  1 << 1);
    TRISFCLR = (  1 <<  6) | (  1 << 4) | (   1 << 3);
}

void display_abcdef_dp (int a, int b, int c, int d, int e, int f, int g, int dp)
{
    PORTDCLR = (! g << 11) | (! f << 8) | (! dp << 7) | (! c << 6) | (! b << 1);
    PORTFCLR = (! d <<  6) | (! e << 4) | (!  a << 3);

    PORTDSET = (  g << 11) | (  f << 8) | (  dp << 7) | (  c << 6) | (  b << 1);
    PORTFSET = (  d <<  6) | (  e << 4) | (   a << 3);
}

void setup_set_digit ()
{
    TRISDCLR = (1 << 5) |  1;
    TRISFCLR = (1 << 2) | (1 << 1);
}

void set_digit (int digit)
{
    switch (digit)
    {
        case 1:

            PORTFCLR = 1 << 2;
            PORTFSET = 1 << 1;
            PORTDSET = (1 << 5) | 1;

            break;

        case 2:

            PORTDCLR = 1;
            PORTDSET = 1 << 5;
            PORTFSET = (1 << 2) | (1 << 1);

            break;

        case 3:

            PORTFCLR = 1 << 1;
            PORTFSET = 1 << 2;
            PORTDSET = (1 << 5) | 1;

            break;

        case 4:

            PORTDCLR = 1 << 5;
            PORTDSET = 1;
            PORTFSET = (1 << 2) | (1 << 1);

            break;
    }
}

void display_digit (int n)
{
    switch (n)
    {
        case 0x0: display_abcdef_dp (1, 1, 1, 1, 1, 1, 0, 0); break;
        case 0x1: display_abcdef_dp (0, 1, 1, 0, 0, 0, 0, 0); break;
        case 0x2: display_abcdef_dp (1, 1, 0, 1, 1, 0, 1, 0); break;
        case 0x3: display_abcdef_dp (1, 1, 1, 1, 0, 0, 1, 0); break;
        case 0x4: display_abcdef_dp (0, 1, 1, 0, 0, 1, 1, 0); break;
        case 0x5: display_abcdef_dp (1, 0, 1, 1, 0, 1, 1, 0); break;
        case 0x6: display_abcdef_dp (1, 0, 1, 1, 1, 1, 1, 0); break;
        case 0x7: display_abcdef_dp (1, 1, 1, 0, 0, 0, 0, 0); break;
        case 0x8: display_abcdef_dp (1, 1, 1, 1, 1, 1, 1, 0); break;
        case 0x9: display_abcdef_dp (1, 1, 1, 1, 0, 1, 1, 0); break;
        case 0xA: display_abcdef_dp (1, 1, 1, 0, 1, 1, 1, 0); break;
        case 0xB: display_abcdef_dp (0, 0, 1, 1, 1, 1, 1, 0); break;
        case 0xC: display_abcdef_dp (1, 0, 0, 1, 1, 1, 0, 0); break;
        case 0xD: display_abcdef_dp (0, 1, 1, 1, 1, 0, 1, 0); break;
        case 0xE: display_abcdef_dp (1, 0, 0, 1, 1, 1, 1, 0); break;
        case 0xF: display_abcdef_dp (1, 0, 0, 0, 1, 1, 1, 0); break;
    }
}

void main (void)
{
    int i, j;

    setup_display_abcdef_dp ();
    setup_set_digit         ();

    for (;;)
    for (i = 0;; i = (i + 1) % 4)
    {
        set_digit (i + 1);

        for (j = 0; j < 16; j++)
        {
            display_digit (j);
            delay (10);
        }
    }
}
