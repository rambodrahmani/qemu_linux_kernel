/**
 * File: hard-disk-1.cpp
 *       This I/O example shows how to write to the primary HDD: it writes a
 *       certain number of sectors to the primary channel on the master drive
 *       starting from the sector addressed by start.
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
    // start sector address
    natl start = 1;

    // amount of sectors to write
    natb sectors = 2;

    // prepare buffer to be written
    char buffer[8*512];
    for (int i = 0; i < sectors*512; i++)
    {
        buffer[i] = 'c';
    }

    // write buffer to hdd
    hdd_write(start, sectors, reinterpret_cast<natw*>(buffer));

    // output 'OK' to the video output
    str_write("OK\n");

    // wait for the ESC key before ending
    pause();

    // return with no errors
    return 0;
}

