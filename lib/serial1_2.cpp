/**
 * File: serial2_o.cpp
 *       Writes the given byte to the COM2 serial interface THR register.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM2.h"

void serial2_o(natb b)
{
    // check if a new byte can be written to THR register
    natb s;
    do
    {
        inputb(iLSR, s);
    } while (! (s & 0x20));

    // write the given byte to the THR register
    outputb(b, iTHR);
}

