/**
 * File: apic_set_IPOL.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_IPOL(natl irq, bool v)
{
    natl work = apic_read_rtl(irq);
    if (v)
        work |= IOAPIC_IPOL_BIT;
    else
        work &= ~IOAPIC_IPOL_BIT;
    apic_write_rtl(irq, work);
}
