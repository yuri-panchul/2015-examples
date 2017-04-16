//  File:   prefetch_cache.c
//  Author: Yuri Panchul

#include <sys/attribs.h>
#include <p32xxxx.h>

#include "prefetch_cache.h"

#define n_cache_lines 16

unsigned       copy_CHELRU;
unsigned       copy_CHEHIT;
unsigned       copy_CHEMIS;
unsigned       copy_CHEPFABT;

__CHETAGbits_t copy_CHETAGbits [n_cache_lines];
unsigned       copy_CHEMSK     [n_cache_lines];
unsigned       copy_CHEW0      [n_cache_lines];
unsigned       copy_CHEW1      [n_cache_lines];
unsigned       copy_CHEW2      [n_cache_lines];
unsigned       copy_CHEW3      [n_cache_lines];

__longramfunc__ void prefetch_cache_backup (void)
{
    int i;

    copy_CHELRU   = CHELRU;
    copy_CHEHIT   = CHEHIT;
    copy_CHEMIS   = CHEMIS;
    copy_CHEPFABT = CHEPFABT;

    for (i = 0; i < n_cache_lines; i ++)
    {
        // CHEACCbits.CHEWEN = 0;  // Write enable
        // CHEACCbits.CHEIDX = i;  // Cache Line Index

        CHEACC = i;

        copy_CHETAGbits [i] = CHETAGbits ;
        copy_CHEMSK     [i] = CHEMSK     ;
        copy_CHEW0      [i] = CHEW0      ;
        copy_CHEW1      [i] = CHEW1      ;
        copy_CHEW2      [i] = CHEW2      ;
        copy_CHEW3      [i] = CHEW3      ;
    }
}

void prefetch_cache_report (bool dump_cache_lines)
{
    int n_cache_lines_to_dump = dump_cache_lines ? n_cache_lines : 0;
    int i;

    printf ("*********  prefetch cache report  *********\n");

    printf ("\nCache Coherency setting on a PFM Program Cycle bit:\n"
            "    %s\n\n",
            CHECONbits.CHECOH ?
                "Invalidate all data and instruction lines"
              : "Invalidate all data lines and instruction lines that are not locked");

    printf ("Data Cache Size: ");

    switch (CHECONbits.DCSZ)
    {
    case 3: printf ( "Enable, 4 lines\n" ); break;
    case 2: printf ( "Enable, 2 lines\n" ); break;
    case 1: printf ( "Enable, 1 line\n"  ); break;
    case 0: printf ( "Disable\n"         ); break;
    }

    printf ("\nPredictive Prefetch Cache:\n");

    switch (CHECONbits.PREFEN)
    {
    case 3: printf ( "    Enable for both cacheable and non-cacheable regions\n\n" ); break;
    case 2: printf ( "    Enable for non-cacheable regions only\n\n"               ); break;
    case 1: printf ( "    Enable for cacheable regions only\n\n"                   ); break;
    case 0: printf ( "    Disable\n\n"                                             ); break;
    }

    printf ("PFM Access Time: %d SYSCLK wait states\n", CHECONbits.PFMWS);

    {
        unsigned CHELRU   = copy_CHELRU;
        unsigned CHEHIT   = copy_CHEHIT;
        unsigned CHEMIS   = copy_CHEMIS;
        unsigned CHEPFABT = copy_CHEPFABT;

        printf ("\nLRU: %.8X\n", CHELRU);

        printf ("\nCounters\n\n");

        printf ("    Hit            : %10d\n", CHEHIT   );
        printf ("    Miss           : %10d\n", CHEMIS   );
        printf ("    Prefetch Abort : %10d\n", CHEPFABT );
    }

    printf ("\nCache lines:\n\n");

    for (i = 0; i < n_cache_lines_to_dump; i ++)
    {
        __CHETAGbits_t CHETAGbits = copy_CHETAGbits [i];
        unsigned       CHEMSK     = copy_CHEMSK     [i];
        unsigned       CHEW0      = copy_CHEW0      [i];
        unsigned       CHEW1      = copy_CHEW1      [i];
        unsigned       CHEW2      = copy_CHEW2      [i];
        unsigned       CHEW3      = copy_CHEW3      [i];

        unsigned address;

        if (CHETAGbits.LTAGBOOT)
            address = 0x1D000000 + (CHETAGbits.LTAG << 4);
        else
            address = 0x1FC00000 + (CHETAGbits.LTAG << 4);

        printf ("    %2d ", i);

        if (! CHETAGbits.LVALID)
        {
            printf ("\n");
            continue;
        }

        printf ("%8X %s %s %s mask: %4X   %.8X:%.8X:%.8X:%.8X\n",
                address,
                CHETAGbits.LVALID ? "valid"       : "     ",
                CHETAGbits.LLOCK  ? "locked"      : "      ",
                CHETAGbits.LTYPE  ? "instruction" : "data       ",
                CHEMSK,
                CHEW0, CHEW1, CHEW2, CHEW3);
    }

    printf ("\n");
    printf ("******  End of prefetch cache report  *****\n\n");
}
