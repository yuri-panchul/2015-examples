//  File:   config.c
//  Author: Yuri Panchul

//////////////////////////////////////////////////////////////////////

// Oscillator settings

//////////////////////////////////////////////////////////////////////

// Oscillator Selection Bits:

// Fast RC Osc (FRC)  
// #pragma config FNOSC = FRC

// Fast RC Osc with PLL  
// #pragma config FNOSC = FRCPLL

// Primary Osc (XT,HS,EC)  
// #pragma config FNOSC = PRI

// Primary Osc w/PLL (XT+,HS+,EC+PLL)  
   #pragma config FNOSC = PRIPLL

// Low Power Secondary Osc (SOSC)  
// #pragma config FNOSC = SOSC

// Low Power RC Osc (LPRC)  
// #pragma config FNOSC = LPRC

// Fast RC Osc w/Div-by-16 (FRC/16)  
// #pragma config FNOSC = FRCDIV16

// Fast RC Osc w/Div-by-N (FRCDIV)  
// #pragma config FNOSC = FRCDIV

//////////////////////////////////////////////////////////////////////

// Primary Oscillator Configuration:

// External clock mode  
// #pragma config POSCMOD = EC

// XT osc mode  
   #pragma config POSCMOD = XT

// HS osc mode  
// #pragma config POSCMOD = HS

// Primary osc disabled  
// #pragma config POSCMOD = OFF

//////////////////////////////////////////////////////////////////////

// PLL Input Divider: 1 2 3 4 5 6 10 12

#pragma config FPLLIDIV = DIV_2

// PLL Multiplier: 15 16 17 18 19 20 21 24

#pragma config FPLLMUL = MUL_20

// System PLL Output Clock Divider: 1 2 4 8 16 32 64 256

#pragma config FPLLODIV = DIV_1

// Peripheral Clock Divisor: Pb_Clk = Sys_Clk / DIV_1, DIV_2, DIV_4, DIV_8
#pragma config FPBDIV = DIV_4

// Secondary Oscillator Enable: ON OFF
#pragma config FSOSCEN = OFF

//////////////////////////////////////////////////////////////////////

// Clock control settings

// Internal/External Switch Over: ON OFF
#pragma config IESO = OFF

//////////////////////////////////////////////////////////////////////

// Clock Switching and Monitor Selection:

// Clock Switch Enable, FSCM Enabled  
// #pragma config FCKSM = CSECME

// Clock Switch Enable, FSCM Disabled  
// #pragma config FCKSM = CSECMD

// Clock Switch Disable, FSCM Disabled  
#pragma config FCKSM = CSDCMD

//////////////////////////////////////////////////////////////////////

// CLKO Output Signal Active on the OSCO Pin: ON OFF
#pragma config OSCIOFNC = OFF

//////////////////////////////////////////////////////////////////////

// Other periferal device settings

// Watchdog Timer Enable: ON, OFF (WDT Disabled (SWDTEN Bit Controls))

#pragma config FWDTEN = OFF

// Watchdog Timer Postscaler: 1:1, 1:2, 1:4, ... 1:65536, 1:1048576

#pragma config WDTPS = PS1024

//////////////////////////////////////////////////////////////////////

// Code protection settings

// Code Protect: ON, OFF
#pragma config CP = OFF

// Boot Flash Write Protect bit: ON, OFF
#pragma config BWP = OFF

// Program Flash Write Protect: PWP512K, PWP508K, ... PWP504K, OFF
#pragma config PWP = OFF

//////////////////////////////////////////////////////////////////////

// Debug settings

// Background Debugger Enable: ON, OFF
#pragma config DEBUG = OFF

//////////////////////////////////////////////////////////////////////

// ICE/ICD Comm Channel Select:

// ICE EMUC1/EMUD1 pins shared with PGC1/PGD1  
// #pragma config ICESEL = ICS_PGx1

// ICE EMUC2/EMUD2 pins shared with PGC2/PGD2  
// #pragma config ICESEL = ICS_PGx2

//////////////////////////////////////////////////////////////////////
