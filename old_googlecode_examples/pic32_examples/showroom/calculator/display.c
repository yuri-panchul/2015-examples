//  File:   display_cls_spi.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "spi.h"
#include "display.h"

static char buf [16];
static int col;

void display_init (void)
{
    spi_init (100000);  // baud rate

    memset (buf, ' ', sizeof (buf));
    col = 0;
}

static void display_scroll ()
{
    int i;

    spi_put_str ("\033[0;0H");  // set cursor position to row 0 column 0

    for (i = 0; i < sizeof (buf); i++)
        spi_put_char (buf [i]);

    spi_put_str ("\033[1;0H");  // set cursor position to row 1 column 0

    for (i = 0; i < sizeof (buf); i++)
        spi_put_char (' ');

    spi_put_str ("\033[1;0H");  // set cursor position to row 1 column 0

    memset (buf, ' ', sizeof (buf));
    col = 0;
}

void display_char (char c)
{
    if (c == '\n')
    {
        display_scroll ();
        return;
    }

    spi_put_char (c);
    buf [col ++] = c;

    if (col == sizeof (buf))
        display_scroll ();
}

void display_str (char *s)
{
    while (*s != '\0')
        display_char (*s++);
}
