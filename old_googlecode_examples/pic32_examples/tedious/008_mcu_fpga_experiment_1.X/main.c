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
    U1BRG           = 80 * 1000000 / 16 / 115200 - 1;
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

    uart_put_nl ();
}

void uart_putn (uchar n)
{
    if (n >= 100)
        uart_putc ('0' + n / 100 % 10);

    if (n >= 10)
        uart_putc ('0' + n / 10 % 10);

    uart_putc ('0' + n % 10);
}

#define n_samples 1000
uchar fpga_data [n_samples];

void fpga_init (void)
{
    int i;

    TRISE = 0xff;  // PORT E is an input
    TRISF = 0;     // PORT F is an output

    PORTF = 0;  // reset

    for (i = 0; i < 1000; i++)
        asm volatile ("nop");

    PORTF = 0xff;  // remove reset

    for (i = 0; i < 1000; i++)
        asm volatile ("nop");
}

void run (void)
{
    int i;
    
    fpga_init ();

    for (i = 0; i < n_samples; i++)
    {
        // fpga_data [i++] = PORTE;
        // asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        // asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        // asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        // asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        // asm volatile ("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop");
        fpga_data [i] = PORTE;
    }
}

void report (void)
{
    int i;

    uart_init ();

    puts ("****************************************");
    puts ("New report");

    for (i = 1; i < n_samples; i++)
    {
        int delta = (int) fpga_data [i] - fpga_data [i - 1];

        if (delta < 0)
            delta += 256;

        uart_putn (delta);

        if (i % 20 != 0)
            uart_putc (' ');
        else
            uart_put_nl ();
    }
}

void main (void)
{
    running_fast ();

    for (;;)
    {
        run ();
        report ();
    }
}
