//  File:   display_oled.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "spi.h"
#include "display.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

#define rows 4
#define cols 16

static char buf [rows - 1][cols];
static int col;

void display_init (void)
{
    OledInit ();
    OledSetCursor (0, rows - 1);

    memset (buf, ' ', sizeof (buf));
    col = 0;
}

static void scroll ()
{
    int row;

    OledSetCharUpdate (0);

    for (row = 0; row < rows; row ++)
    {
        OledSetCursor (0, row);

        for (col = 0; col < cols; col ++)
        {
            char c = row == rows - 1 ? ' ' : buf [row][col];
            OledPutChar (c);

            if (row > 0)
                buf [row - 1][col] = c;
        }
    }

    OledSetCursor (0, rows - 1);
    OledSetCharUpdate (1);

    col = 0;
}

void display_char (char c)
{
    if (c == '\n')
    {
        scroll ();
        return;
    }

    buf [rows - 2][col ++] = c;

    OledPutChar (c);

    if (col == cols)
        scroll ();
    else
        OledSetCursor (col, rows - 1);
}

void display_new_line (void)
{
    display_char ('\n');
}

void display_str (char *s)
{
    while (*s != '\0')
        display_char (*s++);
}

void display_dec (uint n)
{
    uint i;

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            display_char ('0' + n / i % 10);
    }
}

void display_hex_digit (uint n)
{
    char c;

    c  = n & 0x0f;
    c += c >= 10 ? 'A' - 10 : '0';

    display_char (c);
}

void display_hex_byte (char n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        display_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}

void display_hex (uint n)
{
    uint i;

    for (i = 0; i < sizeof (n) * 2; i++)
        display_hex_digit (n >> (sizeof (n) * 2 - 1 - i) * 4);
}
