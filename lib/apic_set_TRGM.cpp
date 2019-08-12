/**
 * File: apic_set_TRGM.cpp
 *       Sets the trigger mode for the specified APIC ir pin:
 *          mode = true   -> IO-APIC-level
 *          mode = false  -> IO-APIC-edge
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_TRGM(natl ir, bool mode)
{
    natl work = apic_read_rtl(ir);

    if (mode)
    {
        work |= IOAPIC_TRGM_BIT;
    }
    else
    {
        work &= ~IOAPIC_TRGM_BIT;
    }

    apic_write_rtl(ir, work);
}

