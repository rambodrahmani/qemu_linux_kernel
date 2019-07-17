/**
 * File: apic_set_MIRQ.cpp
 *       Enables (disable = false) or disables (disable = true) the given
 *       APIC ir pin.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_MIRQ(natl ir, bool disable)
{
    natl work;
    
    // read a long from IDT entry index ir
    *IOREGSEL = 16 + ir * 2;
    work = *IOWIN;
    
    if (disable)
    {
        work |= 0x00010000;     // 16-th bit = 1, disable
    }
    else
    {
        work &= 0xFFFEFFFF;     // 16-th bit = 0, enable
    }

    // write the modified long to the IDT entry index ir
    *IOREGSEL = 16 + ir * 2;
    *IOWIN = work;
}

