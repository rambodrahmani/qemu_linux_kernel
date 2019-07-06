/**
 * File: apic_write_rtl.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_write_rtl(natb irq, natl w)
{
    apic_out(IOAPIC_RTO + irq * 2, w);
}
