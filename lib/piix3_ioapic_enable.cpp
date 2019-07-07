/**
 * File: piix3_ioapic_enable.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "piix3.h"
#include "apic.h"

bool piix3_ioapic_enable(natb bus, natb dev, natb fun)
{
    natl apicbase = pci_read_confb(bus, dev, fun, PIIX3_APICBASE);
    natl xbcs = pci_read_confl(bus, dev, fun, PIIX3_XBCS);
    xbcs |= PIIX3_XBCS_ENABLE;
    pci_write_confl(bus, dev, fun, PIIX3_XBCS, xbcs);
    IOREGSEL = (natl*)(0xFEC00000UL | ((apicbase & 0x1F) << 10));
    IOWIN    = (natl*)(0xFEC00010UL | ((apicbase & 0x1F) << 10));

    return true;
}
