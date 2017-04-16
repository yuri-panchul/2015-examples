//  File:   running.c
//  Author: Yuri Panchul

#include <p32xxxx.h>
#include <plib.h>

#include "config.h"
#include "running.h"
#include "types.h"

void running_fast_1 ()
{
    uint config;
    
    CHECON    = 2;
    BMXCONCLR = 0x40;
    CHECONSET = 0x30;

    asm volatile ("mfc0 %0, $16" : "=r" (config));
    config |= 3;
    asm volatile ("mtc0 %0, $16" : "=r" (config));
}

void running_fast_2 ()
{
    // CHECON [2:0] PFMWS

    CHECONbits.PFMWS = 2;  // Wait states

    // CHECON [5:4] PREFEN: Predictive Prefetch Cache Enable bits
    //
    // 11 = Enable predictive prefetch cache for both cacheable and non-cacheable regions
    // 10 = Enable predictive prefetch cache for non-cacheable regions only
    // 01 = Enable predictive prefetch cache for cacheable regions only
    // 00 = Disable predictive prefetch cache

    CHECONbits.PREFEN = 0;  // Disable predictive prefetch cache

    // CHECON [9:8] DCSZ: Data Cache Size in Lines bits
    //       
    // 11 = Enable data caching with a size of 4 Lines
    // 10 = Enable data caching with a size of 2 Lines
    // 01 = Enable data caching with a size of 1 Line
    // 00 = Disable data caching

    CHECONbits.DCSZ = 0;  // Disable data caching

    // CHECON [6] BMXWSDRM: CPU Instruction or Data Access from Data RAM Wait State bit
    //
    // 1 = Data RAM accesses from CPU have one wait state for address setup
    // 0 = Data RAM accesses from CPU have zero wait states for address setup

    BMXCONbits.BMXWSDRM = 0;

    // CHECON [5:4] PREFEN: Predictive Prefetch Cache Enable bits
    //
    // 11 = Enable predictive prefetch cache for both cacheable and non-cacheable regions
    // 10 = Enable predictive prefetch cache for non-cacheable regions only
    // 01 = Enable predictive prefetch cache for cacheable regions only
    // 00 = Disable predictive prefetch cache

    CHECONbits.PREFEN = 3;  // Enable predictive prefetch cache for both cacheable and non-cacheable regions

    // Cop0 Config Register (CP0 Register 16, Select 0)
    // 2:0 K0: Kseg0 coherency algorithm
    //
    // 2 Uncached
    // 3 Cached

    {
        uint config;

        asm volatile ("mfc0 %0, $16" : "=r" (config));
        config |= 3;
        asm volatile ("mtc0 %0, $16" : "=r" (config));
    }
}

void running_fast_3 ()
{

    SYSTEMConfig (SYSCLK_FREQUENCY, SYS_CFG_ALL); 
}

void running_fast_4 ()
{
    SYSTEMConfigPerformance (SYSCLK_FREQUENCY);
}

void running_fast ()
{
    running_fast_1 ();
}
