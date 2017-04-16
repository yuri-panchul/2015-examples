//  File:   uart.h
//  Author: Yuri Panchul

#ifndef UART_H
#define UART_H

#include "types.h"

void uart_init           (uint baud);

char uart_get_char       (void);

void uart_put_char       (char   c);
void uart_put_new_line   (void    );
void uart_put_str        (char * s);
void uart_put_dec        (uint   n);
void uart_put_hex_digit  (uint   n);
void uart_put_hex_byte   (char   n);
void uart_put_hex        (uint   n);

#endif
