/**
 * File: hdd_rdma.cpp
 *       Please see ../io_examples/bm-hard-disk-1 before this I/O example.
 *
 *       When it comes to bus mastering, a very important example comes from the
 *       ATA bus. Direct transfers between an ATA device and the memory can be
 *       performed using a PCI-ATA bridge. In this case, the bridge itself will
 *       perform as the bus master for the peripheral devices and to this end it
 *       will provide the BMCMD, BMSTR, BMDTPR registers.
 *
 *       This I/O example we will see how to read from the primary hard disk
 *       drive in Bus Mastering.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 28/08/2019.
 */

#include <libqlk.h>

/**
 * Defined in hdd_rdma.s in order to set memory alignment
 */
extern natb buf_rdma[];

/**
 * Reads the amount of sectors defined by 'sectors' from the primary hard disk
 * drive starting from the sector defined by 'sector' in to the memory buffer
 * address by buf.
 *
 * @param  sector   hard disk drive start sector;
 * @param  sectors  number of sectors to read;
 * @param  buf[]    memory buffer address.
 */
extern "C" void go_rdma(natl sector, natb sectors, natb buf[]);

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
    natl lba, sectors;

    // address of the first sector to be read
    lba = 1;

    // number of sector to be read (max 8)
    sectors = 4;

    // call the go_rdma primitive: invoke interrupt 252
    go_rdma(lba, sectors, buf_rdma);

    // loop through the retrived sectors content
    for (int i = 0; i < sectors*512; i++)
    {
        // print the sector content to the video output
        char_write(buf_rdma[i]);
    }

    // write a new line to the video output
    char_write('\n');

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

