/**
 * File: hddw_int.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 21/08/2019.
 */

#include <libqlk.h>

/**
 *
 */
natb buffw[8*512];

/**
 *
 */
extern "C" void go_hddw(natl indse, natb nn, natb vv[]);

/**
 * Developer harness test.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    natl lba, quanti;

    lba = 1;

    quanti = 3;

    for (int i = 0; i < quanti*512; i++)
    {
        buffw[i] = 'i';
    }

    go_hddw(lba, quanti, buffw);

    // write string to the video output
    str_write("OK\n");

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

