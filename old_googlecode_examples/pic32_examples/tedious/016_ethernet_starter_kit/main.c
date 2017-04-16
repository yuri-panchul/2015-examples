/* 
 * File:   main.c
 * Author: panchul
 *
 * Created on February 26, 2013, 4:14 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include <p32xxxx.h>

/*
 * 
 */
int main (int argc, char ** argv)
{
    TRISD = ~ 7;

    for (;;)
    {
        PORTDbits.RD0 = ! PORTDbits.RD6;
        PORTDbits.RD1 = ! PORTDbits.RD7;
        PORTDbits.RD2 = ! PORTDbits.RD13;
    }

    return EXIT_SUCCESS;
}
