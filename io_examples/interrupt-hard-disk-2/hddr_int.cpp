/**
 * File: hddr_int.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 21/08/2019.
 */

#include <libqlk.h>

/**
 * Read buffer.
 */
natb buffr[8*512];

/**
 *
 */
extern "C" void go_hddr(natl indse, natb nn, natb vv[]);

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
    char c;

    natl lba, quanti; 

    lba = 1;

    quanti = 3;

    // retrive data from the hdd into buffr
    go_hddr(lba, quanti, buffr);

    // write the data retrived from the hdd to the video output
    for (int i = 0; i < quanti*512; i++)
    {
        char_write(buffr[i]);
    }

    // write a new line to the video output
    char_write('\n');

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}
