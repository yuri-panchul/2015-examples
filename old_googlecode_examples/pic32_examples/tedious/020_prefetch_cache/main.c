//  File:   main.c
//  Author: Yuri Panchul

//  I am using optimization level 1 for these measurements

#include <sys/attribs.h>
#include <p32xxxx.h>
#include <plib.h>
#include <xc.h>

#include <stdio.h>

#include "config.h"
#include "types.h"
#include "prefetch_cache.h"

//----------------------------------------------------------------------------

#define times_to_repeat   1000
#define n_array_elements  16

#define expected_function_result  \
    ((n_array_elements * (n_array_elements - 1) / 2) * times_to_repeat)

// If you put "const volatile" here,
// the linker puts it into uncacheable region

const int array_in_flash [n_array_elements]
    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

// "volatile" here is necessary so that the compiler
// will not optimize the accesses to this memory away

volatile int array_in_ram [n_array_elements]
    = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

//--------------------------------------------------------------------

int program_in_flash_data_in_flash__loop (void)
{
    const volatile int * a = array_in_flash;

    int i, k, n = 0;

    for (k = 0; k < times_to_repeat; k ++)
    {
        for (i = 0; i < n_array_elements; i ++)
            n += a [i];
    }

    return n;
}

int program_in_flash_data_in_ram____loop (void)
{
    const volatile int * a = array_in_ram;

    int i, k, n = 0;

    for (k = 0; k < times_to_repeat; k ++)
    {
        for (i = 0; i < n_array_elements; i ++)
            n += a [i];
    }

    return n;
}

__longramfunc__ int program_in_ram___data_in_flash__loop (void)
{
    const volatile int * a = array_in_flash;

    int i, k, n = 0;

    for (k = 0; k < times_to_repeat; k ++)
    {
        for (i = 0; i < n_array_elements; i ++)
            n += a [i];
    }

    return n;
}

__longramfunc__ int program_in_ram___data_in_ram____loop (void)
{
    const volatile int * a = array_in_ram;

    int i, k, n = 0;

    for (k = 0; k < times_to_repeat; k ++)
    {
        for (i = 0; i < n_array_elements; i ++)
            n += a [i];
    }

    return n;
}

int program_in_flash_data_in_flash__long_sequence (void)
{
    const volatile int * a = array_in_flash;

    int k, n = 0;

    for (k = 0; k < times_to_repeat / 8; k ++)
    {
        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15];

        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15]
             ;
        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15];

        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15]
             ;
        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15];

        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15]
             ;
        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15];

        n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]
             + a [ 4] + a [ 5] + a [ 6] + a [ 7]
             + a [ 8] + a [ 9] + a [10] + a [11]
             + a [12] + a [13] + a [14] + a [15]
             ;
    }

    return n;
}

#define SEQ  n +=   a [ 0] + a [ 1] + a [ 2] + a [ 3]  \
                  + a [ 4] + a [ 5] + a [ 6] + a [ 7]  \
                  + a [ 8] + a [ 9] + a [10] + a [11]  \
                  + a [12] + a [13] + a [14] + a [15];

#define SEQ_8 SEQ SEQ SEQ SEQ SEQ SEQ SEQ SEQ

int program_in_flash_data_in_ram____long_sequence (void)
{
    const volatile int * a = array_in_ram;

    int k, n = 0;

    for (k = 0; k < times_to_repeat / 8; k ++)
    {
        SEQ_8
    }

    return n;
}

__longramfunc__ int program_in_ram___data_in_flash__long_sequence (void)
{
    const volatile int * a = array_in_flash;

    int k, n = 0;

    for (k = 0; k < times_to_repeat / 8; k ++)
    {
        SEQ_8
    }

    return n;
}

__longramfunc__ int program_in_ram___data_in_ram____long_sequence (void)
{
    const volatile int * a = array_in_ram;

    int k, n = 0;

    for (k = 0; k < times_to_repeat / 8; k ++)
    {
        SEQ_8
    }

    return n;
}

#undef SEQ
#undef SEQ_8

//----------------------------------------------------------------------------

void dump_memory (const volatile void * p)
{
    unsigned         address         = (unsigned) p;
    unsigned         aligned_address = address & ~ 0xF;
    const unsigned * up              = (const unsigned *) aligned_address;

    int i;

    if (address != aligned_address)
    {
        printf ("\n          ");

        for (i = 0; i < 4; i ++)
        {
            if (i % 4 >= address / 4 % 4)
                printf (" ********");
            else
                printf ("         ");
        }
    }

    for (i = 0; i < 32; i ++)
    {
        if (i % 4 == 0)
            printf ("\n%.8X: ", address + i * 4);

        printf (" %.8X", up [i]);
    }

    printf ("\n\n");
}

//----------------------------------------------------------------------------

unsigned double_clock_cycles;

#define n_configurations 6
#define n_functions      8

int configuration_number = 0;
int function_number      = 0;

int test_result_table [n_configurations][n_functions];

__longramfunc__ int run_function_and_backup_cache (int (* f) (void))
{
    int result;

    double_clock_cycles = _CP0_GET_COUNT ();
    result = f ();
    double_clock_cycles = _CP0_GET_COUNT () - double_clock_cycles;
    prefetch_cache_backup ();

    return result;
}

//----------------------------------------------------------------------------

void test_one_function_for_the_current_configuration
(
    int (* function) (void),
    char * function_name
)
{
    int function_result;
    int i;

    // Clear all cache statistics

    CHEHIT   = 0;
    CHEMIS   = 0;
    CHEPFABT = 0;

    // Invalidate all cache lines (does not work with some PIC32s)

    for (i = 0; i < 16; i++)
    {
        CHEACC            = 0x80000000 /* Writable */ | i;
        CHETAGbits.LVALID = 0;
    }

    // Clear counter

    _CP0_SET_COUNT (0);

    // printf ("Check counter: %10u\n", _CP0_GET_COUNT ());

    function_result = run_function_and_backup_cache (function);

    if (function_result != expected_function_result)
    {
        printf ("\n!!! Erratic behavior, result %d != expected %d !!!\n\n",
            function_result, expected_function_result);
    }

    printf ("%-49.49s %10u\n",
        function_name, double_clock_cycles);

    prefetch_cache_report (true);

    printf ("\nFunction dump:\n");
    dump_memory (function);

    if (configuration_number >= n_configurations)
        printf ("\n!!! configuration_number >= n_configurations !!!\n");

    if (function_number >= n_functions)
        printf ("\n!!! function_number >= n_functions !!!\n");

    test_result_table [configuration_number][function_number]
        = double_clock_cycles;

    function_number ++;
}

//----------------------------------------------------------------------------

void test_all_functions_for_the_current_configuration ()
{
    function_number = 0;

    #define TEST(f)  test_one_function_for_the_current_configuration (f, #f);

    TEST ( program_in_flash_data_in_flash__loop          );
    TEST ( program_in_flash_data_in_ram____loop          );
    TEST ( program_in_ram___data_in_flash__loop          );
    TEST ( program_in_ram___data_in_ram____loop          );
    TEST ( program_in_flash_data_in_flash__long_sequence );
    TEST ( program_in_flash_data_in_ram____long_sequence );
    TEST ( program_in_ram___data_in_flash__long_sequence );
    TEST ( program_in_ram___data_in_ram____long_sequence );

    #undef TEST

    configuration_number ++;
}

//----------------------------------------------------------------------------

void main ()
{
    bool alternative_way = false;

    __C32_UART = 1;

    uart_init (9600);

    printf ("************************************************************\n"
            "************************************************************\n"
            "************************************************************\n"
            "*                                                          *\n"
            "*       %s                                        *\n"
            "*       %s                                           *\n"
            "*                                                          *\n"
            "************************************************************\n",
            __DATE__, __TIME__);

    printf ("\narray_in_flash dump:\n");
    dump_memory (array_in_flash);

    printf ("\narray_in_ram dump:\n");
    dump_memory (array_in_ram);
 
    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Default configuration                              *\n"
            "*                                                          *\n"
            "************************************************************\n");

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Lower number of flash wait states                  *\n"
            "*                                                          *\n"
            "************************************************************\n");

    // The default number of flash wait states is 7, it can be lowered.
    // For example: flash can run at 30 MHz, processor at 80 MHz, therefore:
    // (80 + 30 - 1) / 30 = 3 safe wait states

    CHECONbits.PFMWS
        = (SYSCLK_FREQUENCY + FLASH_FREQUENCY - 1) / FLASH_FREQUENCY;

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Enable flash prefetch                              *\n"
            "*                                                          *\n"
            "************************************************************\n");

    CHECONbits.PREFEN = 3;

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Enable flash cache                                 *\n"
            "*                                                          *\n"
            "************************************************************\n");

    if (alternative_way)
    {
        CheKseg0CacheOn ();
    }
    else
    {
        unsigned config = _mfc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT);

        config &= ~ _CP0_CONFIG_K0_MASK;
        config |= 3 << _CP0_CONFIG_K0_POSITION;

        _mtc0 (_CP0_CONFIG, _CP0_CONFIG_SELECT, config);
    }

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Enable flash data caching                          *\n"
            "*                                                          *\n"
            "************************************************************\n");

    CHECONbits.DCSZ = 3;  // Enable data caching with a size of 4 Lines

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Set zero state data RAM access                     *\n"
            "*                                                          *\n"
            "************************************************************\n");

    if (alternative_way)
        mBMXDisableDRMWaitState ();
    else
        BMXCONbits.BMXWSDRM = 0;

    test_all_functions_for_the_current_configuration ();

    printf ("************************************************************\n"
            "*                                                          *\n"
            "*       Dump test result table                             *\n"
            "*                                                          *\n"
            "************************************************************\n");

    printf ("\n"
            "\tProgram Flash, Data Flash, Loop"
            "\tProgram Flash, Data RAM, Loop"
            "\tProgram RAM, Data Flash, Loop"
            "\tProgram RAM, Data RAM, Loop"
            "\tProgram Flash, Data Flash, Sequence"
            "\tProgram Flash, Data RAM, Sequence"
            "\tProgram RAM, Data Flash, Sequence"
            "\tProgram RAM, Data RAM, Sequence");

    printf ("\n                    "
            "\t PF DF L \t PF DR L \t PR DF L \t PR DR L "
            "\t PF DF S \t PF DR S \t PR DF S \t PR DR S ");

    {
        int i, j;

        for (i = 0; i < n_configurations; i ++)
        {
            const char * configuration_names [] =
            {
                "Default              ",
                "Min flash wait states",
                "Flash prefetch       ",
                "Flash program cache  ",
                "Flash data cache     ",
                "Zero wait state RAM  "
            };

            printf ("\n%s", configuration_names [i]);

            for (j = 0; j < n_functions; j ++)
            {
                printf ("\t%8d", test_result_table [i][j]);
            }
        }

        printf ("\n\n");
    }

    printf ("************************************************************\n"
            "************************************************************\n"
            "************************************************************\n");

    for (;;);
}
