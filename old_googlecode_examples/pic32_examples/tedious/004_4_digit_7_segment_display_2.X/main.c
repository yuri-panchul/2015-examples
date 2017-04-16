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

#define pin_a    PORTFbits.RF3
#define pin_b    PORTDbits.RD1
#define pin_c    PORTDbits.RD6
#define pin_d    PORTFbits.RF6
#define pin_e    PORTFbits.RF4
#define pin_f    PORTDbits.RD8
#define pin_g    PORTDbits.RD11
#define pin_dp   PORTDbits.RD7

#define digit_1  PORTFbits.RF2
#define digit_2  PORTDbits.RD0
#define digit_3  PORTFbits.RF1
#define digit_4  PORTDbits.RD5

#define tris_pin_a    TRISFbits.TRISF3
#define tris_pin_b    TRISDbits.TRISD1
#define tris_pin_c    TRISDbits.TRISD6
#define tris_pin_d    TRISFbits.TRISF6
#define tris_pin_e    TRISFbits.TRISF4
#define tris_pin_f    TRISDbits.TRISD8
#define tris_pin_g    TRISDbits.TRISD11
#define tris_pin_dp   TRISDbits.TRISD7

#define tris_digit_1  TRISFbits.TRISF2
#define tris_digit_2  TRISDbits.TRISD0
#define tris_digit_3  TRISFbits.TRISF1
#define tris_digit_4  TRISDbits.TRISD5

void setup_display_abcdef_dp ()
{
    tris_pin_a  = 0;
    tris_pin_b  = 0;
    tris_pin_c  = 0;
    tris_pin_d  = 0;
    tris_pin_e  = 0;
    tris_pin_f  = 0;
    tris_pin_g  = 0;
    tris_pin_dp = 0;
}

void display_abcdef_dp (int a, int b, int c, int d, int e, int f, int g, int dp)
{
    pin_a  = a;
    pin_b  = b;
    pin_c  = c;
    pin_d  = d;
    pin_e  = e;
    pin_f  = f;
    pin_g  = g;
    pin_dp = dp;
}

void setup_set_digit ()
{
    tris_digit_1 = 0;
    tris_digit_2 = 0;
    tris_digit_3 = 0;
    tris_digit_4 = 0;
}

void set_digit (int digit)
{
    digit_1 = digit != 1;
    digit_2 = digit != 2;
    digit_3 = digit != 3;
    digit_4 = digit != 4;
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
