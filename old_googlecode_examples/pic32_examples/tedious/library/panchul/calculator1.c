//  File:   calculator1.c
//  Author: Yuri Panchul

#include "string.h"
#include "types.h"

#define add    0xa
#define sub    0xb
#define mul    0xc
#define div    0xd
#define sign   0xe
#define equal  0xf

static void calculator_int_to_string (int n, char * buf)
{
    uint i;

    if (n < 0)
    {
        * buf ++ = '-';
        n = - n;
    }

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            * buf ++ = '0' + n / i % 10;
    }

    * buf = '\0';
}

static bool calculator_op_to_string (int op, char * buf)
{
    switch (op)
    {
        default:     * buf = '\0'; return false;

        case add:    * buf ++ = '+'; break;
        case sub:    * buf ++ = '-'; break;
        case mul:    * buf ++ = '*'; break;
        case div:    * buf ++ = '/'; break;
        case equal:  break;
    }

    * buf ++ = '\0';
    return true;
}

char * calculator1 (char in)
{
    static char buf [32];

    static int  arg1 = 0;
    static int  arg2 = 0;
    static int  op   = add;

    int n;

    if (in <= 9)
    {
        buf [0] = '0' + in;
        buf [1] = '\0';

        n = arg2 * 10 + in;

        if ((n - in) / 10 != arg2)
            goto OVERFLOW;

        arg2 = n;
    }
    else if (in == sign)
    {
        arg2 = - arg2;

        buf [0] = '=';
        buf [1] = '\0';

        if (! (arg1 == 0 && (op == add || op == sub)))
            calculator_int_to_string (arg1, buf + 1);

        if (! (arg1 == 0 && op == add))
            calculator_op_to_string  (op, buf + strlen (buf));

        calculator_int_to_string (arg2 , buf + strlen (buf));
    }
    else
    {
        switch (op)
        {
            default:

                goto INTERNAL_ERROR;

            case add:

                n = arg1 + arg2;

                if (    arg1 > 0 && arg2 > 0 && n < 0
                     || arg1 < 0 && arg2 < 0 && n > 0)
                {
                    goto OVERFLOW;
                }

                break;

            case sub:

                arg2 = - arg2;

                n = arg1 + arg2;

                if (    arg1 > 0 && arg2 > 0 && n < 0
                     || arg1 < 0 && arg2 < 0 && n > 0)
                {
                    goto OVERFLOW;
                }

                break;

            case mul:

                n = arg1 * arg2;

                if (arg2 != 0 && n / arg2 != arg1)
                    goto OVERFLOW;

                break;

            case div:

                if (arg2 == 0)
                    goto OVERFLOW;

                n = arg1 / arg2;
                break;

            case equal:

                n = 0;
                break;
        }

        buf [0] = '=';

        calculator_int_to_string (n, buf + 1);

        arg1 = n;
        arg2 = 0;

        if (! calculator_op_to_string  (in, buf + strlen (buf)))
            goto INTERNAL_ERROR;

        op = in == equal ? add : in;
    }

    return buf;

    OVERFLOW:

    strcat (buf, "[overflow]");
    goto RESET;

    INTERNAL_ERROR:

    strcat (buf, "[internal error]");
    goto RESET;

    RESET:

    arg1 = 0;
    arg2 = 0;
    op   = add;

    return buf;
}

#ifndef __PIC32MX

#include "conio.h"
#include "ctype.h"
#include "stdio.h"

int main (void)
{
    int c;

    for (;;)
    {
        c = getch ();

        if (isdigit (c))
            c -= '0';
        else if (c >= 'a' && c <= 'f')
            c = c - 'a' + 0xA;
        else if (c >= 'A' && c <= 'F')
            c = c - 'A' + 0xA;
        else if (strchr ("xzq", c) || c == EOF)
            break;
        else
            continue;

        printf ("%s", calculator ((char) c));
    }

    return 0;
}

#endif
