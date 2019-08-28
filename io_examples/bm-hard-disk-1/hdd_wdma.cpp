/**
 * File: hdd_wdma.cpp
 *       When it comes to bus mastering, a very important example comes from the
 *       ATA bus. Direct transfers between an ATA device and the memory can be
 *       performed using a PCI-ATA bridge. In this case, the bridge itself will
 *       perform as the bus master for the peripheral devices and to this end it
 *       will provide the BMCMD, BMSTR, BMDTPR registers.
 *
 *       This I/O example we will see how to write to the primary hard disk
 *       drive in Bus Mastering.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 28/08/2019.
 */

#include <libqlk.h>

/**
 * Memory buffer where to retrieve data to be written to the HDD.
 * Defined in hdd_wdma.s in order to set memory alignment.
 */
extern natb buf_wdma[];

/**
 * Writes to the amount of sectors defined by 'sectors' of the primary hard disk
 * drive, starting from the sector defined by 'sector', the content of the
 * memory buffer address by buf.
 *
 * @param  sector   hard disk drive start sector;
 * @param  sectors  number of sectors to write;
 * @param  buf[]    memory buffer address.
 */
extern "C" void go_wdma(natl sector, natb sectors, natb buf[]);

/**
 * Developer harness test.
 *
 * @param  argc  command line arguments counter.
 * @param  argv  command line arguments.
 *
 * @return       execution exit code.
 */
int main(int argc, char * argv[])
{
    natl lba, sectors;

    // starting sector
    lba = 0;

    // sectors to write to (max 8)
    sectors = 8;

    // loop through the memory buffer
    for (int i = 0; i < sectors*512; i++)
    {
        // initialize each byte with 'd'
        buf_wdma[i] = 'd';
    }

    // write the content of the memory buffer to the hdd
    go_wdma(lba, sectors, buf_wdma);

    // write a new line to the video output
    str_write("OK\n");

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

