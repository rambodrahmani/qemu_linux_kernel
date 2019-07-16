/**
 * File: apic_set_MIRQ.cpp
 *       Enables (disable = false) or disables (enable = true) the given
 *       APIC ir pin.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_MIRQ(natl ir, bool enable)
{
    natl work;
    
    // lettura della prima parola lunga dell'entrata
    // ir-esima della tabella interna
    *IOREGSEL = 16 + ir * 2;
    work = *IOWIN;
    
    if (enable)
    {
        work |= 0x00010000;
    }
    else
    {
        work &= 0xFFFEFFFF;
    }

    // scrittura nella prima parola lunga dell'entrata
    // ir-esima della tabella interna
    *IOREGSEL = 16 + ir * 2;
    *IOWIN = work;
}

