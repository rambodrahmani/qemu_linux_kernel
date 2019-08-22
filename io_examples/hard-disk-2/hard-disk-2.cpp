/**
 * File: hard-disk-2.cpp
 *       This I/O example shows how to read from the primary HDD: it reads a
 *       certain number of sectors from the primary channel on the master drive
 *       starting from the sector addressed by start, and outputs the data to
 *       the video output.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libqlk.h>
#include <hdd.h>

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
    // start sector to read from
    natl start = 1;

    // amount of sectors to read
    natb sectors = 2;

    // read buffer
    natb buffer[8*512];

    // read from the hdd
    hdd_read(start, sectors, reinterpret_cast<natw*>(buffer));

    // output to the video output
    for (int i = 0; i < sectors*512; i++)
    {
        char_write(buffer[i]);
    }

    // write a new line to the video output
    char_write('\n');

    // wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

