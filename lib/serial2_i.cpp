/**
 * File: serial2_i.cpp
 *       Reads a byte from the COM2 serial interface RBR register.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 15/07/2019.
 */

#include "libqlk.h"
#include "COM2.h"

void serial2_i(natb &b)
{
    // check if a new byte is available in the RBR register
    natb s;
    do
    {
        inputb(iLSR, s);
    } while (! (s & 0x01));

    // read the byte from the RBR register
    inputb(iRBR, b);
}

