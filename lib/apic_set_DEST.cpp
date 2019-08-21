/**
 * File: apic_set_DEST.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_DEST(natl irq, natb dest)
{
    natl work = apic_read_rth(irq);
    
    work = (work & ~IOAPIC_DEST_MSK) | (dest << IOAPIC_DEST_SHF);
    
    apic_write_rth(irq, work);
}

