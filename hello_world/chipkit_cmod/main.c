#include <p32xxxx.h>

void delay (int n)
{
    n *= 1000;

    while (n --)
        asm volatile ("nop");
}

void main (void)
{
    TRISB = 0;

    for (;;)
    {
        PORTB = ~ 0;
        delay (50);
        PORTB = 0;
        delay (50);
    }
}
