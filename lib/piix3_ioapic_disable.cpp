/**
 * File: piix3_ioapic_disable.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "piix3.h"
#include "pci.h"

bool piix3_ioapic_disable(natb bus, natb dev, natb fun)
{
    natl xbcs = pci_read_confl(bus, dev, fun, PIIX3_XBCS);
    
    xbcs &= ~PIIX3_XBCS_ENABLE;

    pci_write_confl(bus, dev, fun, PIIX3_XBCS, xbcs);

    return true;
}

