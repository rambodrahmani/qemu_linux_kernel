/**
 * File: apic_read_rtl.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

natl apic_read_rtl(natb irq)
{
    return apic_in(IOAPIC_RTO + irq * 2);
}
