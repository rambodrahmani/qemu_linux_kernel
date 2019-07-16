/**
 * File: hdd_read.cpp
 *       Reads into data from the specified HDD sectors starting from the
 *       specified start sector.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "hdd.h"

void hdd_read(natl start, natb sectors, natw data[])
{	
    natb start_0 = start,
         start_1 = start >> 8,
         start_2 = start >> 16,
         start_3 = start >> 24;
	
    // check if the disk controller is not busy: otherwise wait
    natb s;
	do
    {
        inputb(iSTS, s);
    } while (s & 0x80);

    // set start sector address
    outputb(start_0, iSNR);

    // set cylinder number (low)
    outputb(start_1, iCNL);

    // set cylinder number (high)
    outputb(start_2, iCNH);

    // select drive
    natb hnd = (start_3 & 0x0F) | 0xE0;
    outputb(hnd, iHND);

    // set sectors number
    outputb(sectors, iSCR);

    // disable interrupt requests
    outputb(0x0A, iDCR);

    // set the read command
    outputb(0x20, iCMD);

    // read the data from the buffer register
    for (int i = 0; i < sectors; i++)
    {
        // check if the disk controller is not busy and has available data
        do
        {
            inputb(iSTS, s);
        } while ((s & 0x88) != 0x08);

        // read the data from the disk controller buffer register
        for (int j = 0; j < 512/2; j++)
        {
            inputw(iBR, data[i*512/2 + j]);
        }
    }
}
