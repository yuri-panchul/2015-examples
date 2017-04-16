//  File:   main.c
//  Author: Yuri Panchul

#include "calculator.h"
#include "display.h"
#include "keypad.h"
#include "running.h"
#include "types.h"

void main (void)
{
    int i;

    running_fast ();
    display_init ();
    keypad_init  (true);  // use_interrupts

    display_str ("Calculator");

    for (;;)
        display_str (calculator (keypad_get ()));
}
