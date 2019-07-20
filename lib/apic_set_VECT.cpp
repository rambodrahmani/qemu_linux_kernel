/**
 * File: apic_set_VECT.cpp
 *       Sets the given type for the interrupts coming from the specified APIC
 *       ir pin.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_VECT(natl ir, natb type)
{
    natl work;

    // read a long from IDT entry index ir
    *IOREGSEL = 16 + ir * 2;          // write APIC internal register address
    work = *IOWIN;                    // retrieve APIC internal register content

    // zero out bits 7-0
    work &= 0xFFFFFF00;

    // set bits 7-0 with the new type
    work |= type;

    // write the modified long to the IDT entry index ir
    *IOREGSEL = 16 + ir * 2;          // write APIC internal register address
    *IOWIN = work;                    // retrieve APIC internal register content
}

