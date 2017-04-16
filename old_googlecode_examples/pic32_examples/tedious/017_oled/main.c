//  File:   main.c
//  Author: Yuri Panchul

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

#include "delay.h"
#include "display.h"

#define width   ccolOledMax
#define height  crowOledMax

void draw_sin_and_cos ()
{
    int x;
    double pi = atan (1) * 4;

    OledClear ();

    for (x = 0; x < width; x++)
    {
        int y = height / 2 - (int) (sin (x * pi * 10 / width) * (height / 2 - 1));

        if (x != 0)
            OledLineTo (x, y);

        OledMoveTo (x, y);
    }

    OledUpdate ();

    for (x = 0; x < width; x++)
    {
        int y = height / 2 - (int) (cos (x * pi * 3 / width) * (height / 2 - 1));

        if (x != 0)
            OledLineTo (x, y);

        OledMoveTo (x, y);
        OledUpdate ();
    }
}

void draw_random_rectangles ()
{
    int k, i;

    OledClear ();

    for (k = 2; k >= 0; k --)
    {
        for (i = 0; i < 100; i ++)
        {
            int x = rand () % width;
            int y = rand () % height;

            OledMoveTo   (x, y);
            OledDrawRect (x + rand () % (width - x), y + rand () % (height - y));

            OledUpdate ();
        }

        OledSetDrawMode (modOledXor);
    }
}

void demo_character_display ()
{
    int i;
    
    display_init ();

    for (i = 0; i < 200; i++)
        display_char ('a' + i % 26);
}

void main ()
{
    unsigned i;

    OledInit ();

    for (;;)
    {
        draw_sin_and_cos        ();
        draw_random_rectangles  ();
        demo_character_display  ();
    }
}
