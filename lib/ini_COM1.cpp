/**
 * File: ini_COM1.cpp
 *       Initializes serial interface COM1:
 *          bit-rate:    9600 bit/s
 *          frame:       8 bits
 *          parity bit:  disabled
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM1.h"

void ini_COM1()
{
    // dummy bit
    natb dymmy;

    // 9600 bits/sec
    natw CBITR = 0x000C;

    // DLAB = 1
    outputb(0x80, iLCR);

    outputb(CBITR, iDLR_LSB);

    outputb(CBITR >> 8, iDLR_MSB);

    // 1 stop-bit, 8-bits frame, no parity bit
    outputb(0x03, iLCR);

    // interrupt requests disabled
    outputb(0x00, iIER);

    // clean up the RBR buffer
    inputb(iRBR, dummy);
}

