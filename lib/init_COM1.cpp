/**
 * File: init_COM1.cpp
 *       Initializes serial interface COM1:
 *          bit-rate:    9600 bit/s
 *          frame:       start-bit:  1
 *                       character:  8 bits
 *                       parity bit: 0
 *                       stop-bit:   1
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM1.h"

void init_COM1()
{
    // dummy bit
    natb dummy;

    // 9600 bits/sec
    natw CBITR = 0x000C;

    // DLAB = 1, select DLR register
    outputb(0x80, iLCR);

    // write time constant LSB
    outputb(CBITR, iDLR_LSB);

    // write time constant MSB
    outputb(CBITR >> 8, iDLR_MSB);

    // 1 stop-bit, 8-bits character, no parity bit
    outputb(0x03, iLCR);

    // interrupt requests disabled
    outputb(0x00, iIER);

    // clean up the RBR buffer
    inputb(iRBR, dummy);
}

