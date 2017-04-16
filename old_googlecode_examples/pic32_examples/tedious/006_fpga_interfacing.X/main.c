//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

typedef unsigned uint;

void delay (int n)
{
    n *= 10000;

    while (n --)
        asm volatile ("nop");
}

#define bin(a, b, c, d, e, f, g, dp)  \
    (  (a << 7) | (b << 6) | (c << 5) | (d << 4)  \
     | (e << 3) | (f << 2) | (g << 1) |  dp )

uint encode_digit (uint n)
{
    switch (n)
    {
        case 0x0: return bin (1, 1, 1, 1, 1, 1, 0, 0);
        case 0x1: return bin (0, 1, 1, 0, 0, 0, 0, 0);
        case 0x2: return bin (1, 1, 0, 1, 1, 0, 1, 0);
        case 0x3: return bin (1, 1, 1, 1, 0, 0, 1, 0);
        case 0x4: return bin (0, 1, 1, 0, 0, 1, 1, 0);
        case 0x5: return bin (1, 0, 1, 1, 0, 1, 1, 0);
        case 0x6: return bin (1, 0, 1, 1, 1, 1, 1, 0);
        case 0x7: return bin (1, 1, 1, 0, 0, 0, 0, 0);
        case 0x8: return bin (1, 1, 1, 1, 1, 1, 1, 0);
        case 0x9: return bin (1, 1, 1, 1, 0, 1, 1, 0);
        case 0xA: return bin (1, 1, 1, 0, 1, 1, 1, 0);
        case 0xB: return bin (0, 0, 1, 1, 1, 1, 1, 0);
        case 0xC: return bin (1, 0, 0, 1, 1, 1, 0, 0);
        case 0xD: return bin (0, 1, 1, 1, 1, 0, 1, 0);
        case 0xE: return bin (1, 0, 0, 1, 1, 1, 1, 0);
        case 0xF: return bin (1, 0, 0, 0, 1, 1, 1, 0);
    }
}

#undef bin

void main (void)
{
    int i;

    TRISE = 0;
    PORTE = 0xff;

    // Reset

    TRISF = 0;

    for (i = 0; i < 16; i ++)
        PORTF = 0;

    PORTF = 0xff;

    for (;;)
    for (i = 0; i < 16; i ++)
    {
        PORTE = encode_digit (i); delay (10);
    }
}
