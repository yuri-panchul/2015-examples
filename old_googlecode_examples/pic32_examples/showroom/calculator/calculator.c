//  File:   calculator.c
//  Author: Yuri Panchul

#include <string.h>

#include "types.h"

//
//  This code uses short identifiers, static fixed sized buffers,
//  no C++ features (like namespaces), no consideration about portability etc.
//  The reason is straightforward: this code is intended to be a very small
//  easy to read example that is a part of a small program that illustrates
//  the features of a microcontroller.
//
//  This code is neither a part of a large industrial project
//  nor an example how to write portable reusable code.
//  Therefore: less text = easier to read and to follow.
//

////////////////////////////////////////////////////////////////////////////

enum
{
    add          = 0xa,
    substract    = 0xb,
    multiply     = 0xc,
    divide       = 0xd,
    parentheses  = 0xe,
    equal        = 0xf
};

////////////////////////////////////////////////////////////////////////////

static char buf [128] = "\n";

#define newline_before_text  buf
#define text_start           (buf + 1)

static char * cur = text_start;

////////////////////////////////////////////////////////////////////////////

//
//  int_to_string is my alternative to itoa. I wrote int_to_string
//  because itoa did not work for me in MPLAB X PIC32 package.
//  itoa is also not an ANSI/ISO standard function and is not re-enterant.
//  Yes, I am aware that "i = 1000 * 1000 * 1000" is not portable.
//

static void int_to_string (int n, char * s)
{
    uint i;

    if (n < 0)
    {
        * s ++ = '-';
        n = - n;
    }

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            * s ++ = '0' + n / i % 10;
    }

    * s = '\0';
}

////////////////////////////////////////////////////////////////////////////

//
//  The expression parser / evaluator is implemented using simple top-down
//  recursion. A more reliable and table-driven way would be to write
//  a bottom-up operator precedence parser.
//

static bool expr1 (int * pn);
static bool expr2 (int * pn);
static bool expr3 (int * pn);

////////////////////////////////////////////////////////////////////////////

static bool expr1 (int * pn)
{
    int op, n1, n2, n;

    if (! expr2 (& n1))
        return false;

    op = * cur;

    if (op != '+' && op != '-')
    {
        * pn = n1;
        return true;
    }

    cur ++;

    if (! expr1 (& n2))
        return false;

    if (op == '-')
        n2 = - n2;

    n = n1 + n2;

    // checking overflow

    if (    n1 > 0 && n2 > 0 && n < 0
         || n1 < 0 && n2 < 0 && n > 0)
    {
        return false;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr2 (int * pn)
{
    int op, n1, n2, n;

    if (! expr3 (& n1))
        return false;

    op = * cur;

    if (op != '*' && op != '/')
    {
        * pn = n1;
        return true;
    }

    cur ++;

    if (! expr2 (& n2))
        return false;

    if (op == '*')
    {
        n = n1 * n2;
 
        // checking overflow

        if (n2 != 0 && n / n2 != n1)
            return false;
    }
    else
    {
        if (n2 == 0)
            return false;

        n = n1 / n2;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr3 (int * pn)
{
    if (* cur == '-' && (cur == text_start || strchr ("+-*/(", cur [-1])))
    {
        cur ++;

        if (! expr3 (pn))
            return false;

        * pn = - *pn;
    }
    else if (* cur == '(')
    {
        cur ++;

        if (! expr1 (pn))
            return false;

        if (* cur != ')')
            return false;

        cur ++;
    }
    else if (isdigit (* cur))
    {
        int n = 0;

        do
        {
            int d  = * cur - '0';
            int nn = n * 10 + d;

            // checking overflow

            if ((nn - d) / 10 != n)
                return false;

            n = nn;
        }
        while (isdigit (* ++ cur));

        * pn = n;
    }
    else
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

char * calculator (char in)
{
    static char prev_in = equal;

    switch (in)
    {
    case add       :
    case substract :
    case multiply  :
    case divide    :

        break;

    default:

        if (prev_in == equal)
            cur = text_start;

        break;
    }

    prev_in = in;

    if (in == equal)
    {
        int n;

        cur = text_start;

        if (expr1 (& n) && * cur == '\0')
        {
            int_to_string (n, text_start);
            cur = text_start + strlen (text_start);
        }
        else
        {
            strcpy (text_start, "error @ ");

            int_to_string
            (
                cur - text_start + 1,
                text_start + strlen (text_start)
            );

            cur = text_start;
        }

        return newline_before_text;
    }

    switch (in)
    {
    case add        : in = '+'; break;
    case substract  : in = '-'; break;
    case multiply   : in = '*'; break;
    case divide     : in = '/'; break;

    case parentheses:

        if (cur == text_start || strchr ("+-*/(", cur [-1]))
            in = '(';
        else
            in = ')';

        break;

    default:
        
        in = '0' + in;
        break;
    }

    if (cur == buf + sizeof (buf) - 2)
    {
        strcpy (text_start, "buffer overflow @ ");

        int_to_string
        (
            cur - text_start + 1,
            text_start + strlen (text_start)
        );

        return newline_before_text;
    }

    * cur ++ = in;
    * cur    = '\0';

    return cur == text_start + 1 ? newline_before_text : cur - 1;
}

////////////////////////////////////////////////////////////////////////////

#ifndef __PIC32MX

#include "conio.h"
#include "ctype.h"
#include "stdio.h"

int main (void)
{
    char c;

    for (;;)
    {
        c = getch ();

        if (strchr ("zq.", c) != NULL)
            break;

        switch (c)
        {
        case '+'  : c = add         ; break;
        case '-'  : c = substract   ; break;
        case '*'  : c = multiply    ; break;
        case '/'  : c = divide      ; break;
        case '('  : c = parentheses ; break;
        case ')'  : c = parentheses ; break;
        default   : c = c - '0'     ; break;
        }

        printf ("%s", calculator (c));
    }

    return 0;
}

#endif
