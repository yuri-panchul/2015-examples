//  File:   memory.c
//  Author: Yuri Panchul

#include <stdlib.h>
#include <sys/attribs.h>
#include <p32xxxx.h>

#include "types.h"
#include "memory.h"

//--------------------------------------------------------------------

static void memory_partition_dump (uint virtual_address, uint size)
{
    uint * start        = (uint *) virtual_address;
    uint * end          = start + size / sizeof (uint);

    uint   block_size   = 1024 / sizeof (uint);
    uint * block_end    = start + block_size;

    uint   column       = 0;
    uint   max_columns  = 64;

    uint * p            = start;
    uint   n;

    printf ("\nDump:   ");

    for (;;)
    {
        n = 0;

        while (p < block_end)
        {
            if (* p != 0 && * p != ~ 0)
                n ++;

            p ++;
        }

        if (n == 0)
        {
            printf (".");
        }
        else
        {
            uint d = max (1, n * 9 / block_size);
            printf ("%c", '0' + d, n);
        }

        if (p == end)
            break;

        if (++ column == max_columns)
        {
            printf ("\n        ");
            column = 0;
        }

        block_end += block_size;

        if (block_end > end)
            block_end = end;
    }

    printf ("\n\n");
}

//--------------------------------------------------------------------

static void memory_partition_report
(
    bool   kernel,
    char * name,
    uint   virtual_address,
    uint   size,
    bool   dump
)
{
    uint physical_address
      = kernel
        ? virtual_address & ~ 0xE0000000
        : virtual_address +   0x40000000;

    printf ("%-40s  ", name);

    if ((virtual_address & 0x80000000) == 0)
        printf ("kuseg   ");
    else
        printf ("kseg%d   ", (virtual_address >> 29) & 3);

    if (size == 0)
    {
        printf ("                                            none\n");
    }
    else
    {
        printf ("%08x-%08x  %08x-%08x  %10u  %7u KB\n",
            virtual_address,  virtual_address  + size - 1,
            physical_address, physical_address + size - 1,
            size, size / 1024);
    }

    if (dump && size > 0 && size < 1024 * 1024)
        memory_partition_dump (virtual_address, size);
}

//--------------------------------------------------------------------

void memory_partitions_report (bool dump)
{
    bool no_optional_data_partitions
        = (BMXDKPBA == 0) || (BMXDUDBA == 0) || (BMXDUPBA == 0);

    if (dump)
        printf ("*****  Memory map and dump  ************\n\n");
    else
        printf ("**********  Memory map  ****************\n\n");

    printf ("                                                  Virtual            Physical                 Size\n");

    memory_partition_report
    (
        true,
        "Boot Flash",
        0xBFC00000,
        BMXBOOTSZ,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Program Flash cacheable",
        0x9D000000,
        BMXPUPBA == 0 ? BMXPFMSZ : BMXPUPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Program Flash non-cacheable",
        0xBD000000,
        BMXPUPBA == 0 ? BMXPFMSZ : BMXPUPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Data RAM",
        0x80000000,
        no_optional_data_partitions ? BMXDRMSZ : BMXDKPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Data RAM",
        0xA0000000,
        no_optional_data_partitions ? BMXDRMSZ : BMXDKPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Program RAM",
        0x80000000 + BMXDKPBA,
        no_optional_data_partitions ? 0 : BMXDUDBA - BMXDKPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Kernel Program RAM",
        0xA0000000 + BMXDKPBA,
        no_optional_data_partitions ? 0 : BMXDUDBA - BMXDKPBA,
        dump
    );

    memory_partition_report
    (
        true,
        "Peripheral",
        0xBF800000,
        1024 * 1024,
        false  // don't dump
    );

    memory_partition_report
    (
        false,
        "User Program Flash",
        0x7D000000 + BMXPUPBA,
        BMXPUPBA == 0 ? 0 : BMXPFMSZ - BMXPUPBA,
        dump
    );

    memory_partition_report
    (
        false,
        "User Data RAM",
        0x7F000000 + BMXDUDBA,
        no_optional_data_partitions ? 0 : BMXDUPBA - BMXDUDBA,
        dump
    );

    memory_partition_report
    (
        false,
        "User Program RAM",
        0x7F000000 + BMXDUPBA,
        no_optional_data_partitions ? 0 : BMXDRMSZ - BMXDUPBA,
        dump
    );

    printf ("\n");
}

//--------------------------------------------------------------------

const int memory_test_const_variable = 2;
      int memory_test_variable       = 3;

__ramfunc__ int memory_test_ramfunc (int a, int b)
{
    return a * b * memory_test_const_variable * memory_test_variable;
}

__longramfunc__ int memory_test_longramfunc (int a, int b)
{
    return memory_test_ramfunc (a, b);
}

int memory_test_regular_function (int a, int b)
{
    return memory_test_longramfunc (a, b);
}

//--------------------------------------------------------------------

void memory_report (void)
{
    printf ("************  Memory report  ***********\n\n");

    printf ("Flash memory data cacheability for DMA accesses is %s\n",
            BMXCONbits.BMXCHEDMA ? "ENABLED" : "DISABLED");

    if (BMXCONbits.BMXCHEDMA)
        printf ("(requires cache to have data caching enabled)\n\n");
    else
        printf ("(hits are still read from the cache, but misses do not update the cache)\n\n");

    printf ("Bus error enables: %d %d %d %d %d\n",
            BMXCONbits.BMXERRIXI,
            BMXCONbits.BMXERRICD,
            BMXCONbits.BMXERRDMA,
            BMXCONbits.BMXERRDS,
            BMXCONbits.BMXERRIS);

    printf ("\nNumber of wait states for address setup for data RAM accesses: %d\n",
            BMXCONbits.BMXWSDRM);

    if (BMXCONbits.BMXWSDRM)
        printf ("(slower but useful for debug)\n\n");
    else
        printf ("(fast but has a problem with debugging data breakpoint)\n\n");

    printf ("Bus Matrix Arbitration Mode: %d\n",
            BMXCONbits.BMXARB);

    switch (BMXCONbits.BMXARB)
    {
        case 0:

            printf ("Debug > Data > Instruction > DMA > Expansion\n");
            printf ("May starve DMA if DMA is used\n\n");
            break;

        case 1:

            printf ("Debug > Data > DMA > Expansion > Instruction\n\n");
            break;

        case 2:

            printf ("Rotating Priority Sequence\n\n");
            break;
    }

    memory_partitions_report (false);
    memory_partitions_report (true);

    printf ("**********  Functions in RAM  **********\n\n");

    printf ("Address of memory_test_const_variable        : %8x\n",
            (uint) & memory_test_const_variable);

    printf ("Address of memory_test_variable              : %8x\n",
            (uint) & memory_test_variable);

    printf ("Address of memory_test_longramfunc           : %8x\n",
            (uint) memory_test_longramfunc);

    printf ("Address of memory_test_ramfunc               : %8x\n",
            (uint) memory_test_ramfunc);

    printf ("Address of memory_test_regular_function      : %8x\n",
            (uint) memory_test_regular_function);

    printf ("\n"
            "Calling memory_test_regular_function (2, 3)  : %d\n",
            (uint) memory_test_regular_function (2, 3));

    printf ("\n****************************************\n");
}
