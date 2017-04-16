//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "calculator.h"
#include "config.h"
#include "delay.h"
#include "memory.h"
#include "running.h"
#include "types.h"
#include "spi.h"
#include "uart.h"

void main (void)
{
    int i;

    running_fast ();
    display_init ();
    keypad_init  (true);  // use_interrupts

    display_str ("Calculator");

    for (;;)
        display_str (calculator2 (keypad_get ()));
}
