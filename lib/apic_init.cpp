/**
 * File: apic_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"
#include "piix3.h"

extern "C" void disable_8259();

bool apic_init()
{
    natb bus = 0, dev = 0, fun = 0;

    if (! pci_find_dev(bus, dev, fun, PIIX3_DEVICE_ID, PIIX3_VENDOR_ID))
    {
        printf("PIIX3 non trovato\n");
        return false;
    }

    if (! piix3_ioapic_enable(bus, dev, fun))
    {
        printf("IOAPIC non abilitato\n");
        return false;
    }

    disable_8259();

    return true;
}
