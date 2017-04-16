//  File:   delay.h
//  Author: Yuri Panchul

#ifndef DELAY_H
#define DELAY_H

#include "types.h"

void delay_for_1000_nops_x (uint n);
void delay_for_1000_nops   (void);
void delay_micros          (uint n);
void delay_millis          (uint n);
void delay_seconds         (uint n);

#endif
