/**
 * File: apic_reset.cpp
 *       Resets the APIC pins.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_reset()
{
    for (natb i = 0; i < IRQ_MAX; i++)
    {
        apic_write_rth(i, 0);
        apic_write_rtl(i, IOAPIC_MIRQ_BIT | IOAPIC_TRGM_BIT);
    }

    apic_set_TRGM(0, false);
    apic_set_TRGM(2, false);
}

