//  File:   uart.h
//  Author: Yuri Panchul

#ifndef UART_H
#define UART_H

#include "types.h"

void uart_init (uint hertz, uint baud);

void uart_put_char      (uchar   c);
void uart_put_new_line  (void     );
void uart_put_str       (uchar * s);
void uart_put_dec       (uint    n);
void uart_put_hex_digit (uint    n);
void uart_put_hex       (uint    n);

#endif
