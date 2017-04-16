//  File:   main.c
//  Author: Yuri Panchul

#include <p32xxxx.h>

typedef unsigned int  uint;
typedef unsigned char uchar;

// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
//
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1

void running_fast ()
{
    uint config;
    
    CHECON    = 2;
    BMXCONCLR = 0x40;
    CHECONSET = 0x30;

    asm volatile ("mfc0 %0, $16" : "=r" (config));
    config |= 3;
    asm volatile ("mtc0 %0, $16" : "=r" (config));
}

void uart_init (void)
{
    U1STAbits.UTXEN = 1;   // enable transmit pin
    U1STAbits.URXEN = 1;   // enable receive pin
    U1BRG           = 80 * 1000000 / 16 / 115200 - 1;  // 80 MHz, 115.2K baud
    U1MODEbits.ON   = 1;   // enable UART
}

void uart_putc (uchar c)
{
    while (U1STAbits.UTXBF);  // wait until transmit buffer empty
    U1TXREG = c;              // transmit character over UART
}

void uart_put_nl (void)
{
    uart_putc ('\r');
    uart_putc ('\n');
}

void uart_puts (uchar *s)
{
    while (*s != '\0')
        uart_putc (*s++);
}

void uart_putn (uchar n)
{
    if (n >= 100)
        uart_putc ('0' + n / 100 % 10);

    if (n >= 10)
        uart_putc ('0' + n / 10 % 10);

    uart_putc ('0' + n % 10);
}

#define PORTF_RESET         0x02
#define PORTF_INPUT_STROBE  0x04

void fpga_init (void)
{
    int i;

    TRISE =   0;  // PORT E is an output
    TRISF =   0;  // PORT F is an output
    TRISD = ~ 0;  // PORT D is an input

    for (i = 0; i < 1000; i++)
        asm volatile ("nop");

    PORTF = ~ PORTF_RESET;

    for (i = 0; i < 1000; i++)
        asm volatile ("nop");

    PORTF = PORTF_RESET;  // remove reset

    for (i = 0; i < 1000; i++)
        asm volatile ("nop");
}

uint calculate_expected_result (uint n)
{
    return ((n >> 4) & 0xC) | (n & 0x3);
}

void output_result
(
    uint number_of_nops,
    uint argument,
    uint result    
)
{
    uint expected_result = calculate_expected_result (argument);

    if (result == expected_result)
        return;

    uart_putn   (number_of_nops);
    uart_puts   (" nops ");
    uart_putn   (argument);
    uart_putc   (' ');
    uart_putn   (result);
    uart_putc   (' ');
    uart_putn   (expected_result);
    uart_put_nl ();
}

void run (void)
{
    uint n, r;
    
    fpga_init ();
    uart_init ();

    for (n = 0; n < 256; n++)
    {
        if (1) {
        PORTE = n;
        r = PORTD & 0xF;
        output_result (0, n, r);

        PORTE = n;
        asm volatile ("nop");
        r = PORTD & 0xF;
        output_result (1, n, r);

        PORTE = n;
        asm volatile ("nop; nop");
        r = PORTD & 0xF;
        output_result (2, n, r);
        }

        PORTE = n;
        asm volatile ("nop; nop; nop");
        r = PORTD & 0xF;
        output_result (3, n, r);

        PORTE = n;
        asm volatile ("nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (4, n, r);

        PORTE = n;
        asm volatile ("nop; nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (5, n, r);

        PORTE = n;
        asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        r = PORTD & 0xF;
        output_result (10, n, r);
    }
}

void main (void)
{
    running_fast ();

    for (;;)
        run ();
}
