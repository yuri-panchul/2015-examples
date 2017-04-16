//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

#include "config.h"
#include "delay.h"
#include "running.h"
#include "types.h"
#include "uart.h"

#define PORTD_DELAY_VALUE  (PORTD >> 8)  // I/O Shield Switches

#define PORTD_NO_RESET     0x20
#define PORTD_TAG          0x40

void fpga_init (void)
{
    int i;

    AD1PCFG = ~0;  // No analog ports

    TRISE = 0x00;   // PORT E is an output
    TRISD = 0xF1F;  // PORT D [11:8] - input, D [7:5] - output, [3:0] - input

    delay_millis (1);

    PORTD = ~ PORTD_NO_RESET;

    delay_millis (1);

    PORTD = PORTD_NO_RESET;

    delay_millis (1);
}

uint calculate_expected_result (uint n)
{
    n = (n * n) + 3;
    return (n * n) & 0xF;
}

void output_result
(
    uint number_of_nops,
    uint argument,
    uint result    
)
{
    uint expected_result = calculate_expected_result (argument);

    // if (result == expected_result)
    //     return;

    uart_put_dec  (number_of_nops);
    uart_put_str  (" nops ");
    uart_put_dec  (argument);
    uart_put_char (' ');
    uart_put_dec  (result);

    if (result == expected_result)
    {
        uart_put_str (" ok");
    }
    else
    {
        uart_put_str (" bad ");
        uart_put_dec (expected_result);
    }

    uart_put_new_line ();
    delay_millis (PORTD_DELAY_VALUE * 100);
}

void run (void)
{
    uint n, r;

    uart_init (PBCLK_FREQUENCY, 9600);

    fpga_init ();

    for (n = 0; n < 256; n++)
    {
        PORTE = n;
        PORTD = PORTD_NO_RESET | (n & 1 ? 0 : PORTD_TAG);
        asm volatile ("nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (4, n, r);
    }

    fpga_init ();

    for (n = 0; n < 256; n++)
    {
        PORTE = n;
        PORTD = PORTD_NO_RESET | (n & 1 ? 0 : PORTD_TAG);
        asm volatile ("nop; nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (5, n, r);
    }

    fpga_init ();

    for (n = 0; n < 256; n++)
    {
        PORTE = n;
        PORTD = PORTD_NO_RESET | (n & 1 ? 0 : PORTD_TAG);
        asm volatile ("nop; nop; nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (6, n, r);
    }

    delay_seconds (1);
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
