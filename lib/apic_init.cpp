/**
 * File: apic_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"
#include "piix3.h"
#include "pci.h"

/**
 * See 64/8259.s
 */
extern "C" void disable_8259();

bool apic_init()
{
    // start from bus 0, device 0, function 0
    natb bus = 0, dev = 0, fun = 0;

    // look for PIIX3 device
    if (! pci_find_dev(bus, dev, fun, PIIX3_DEVICE_ID, PIIX3_VENDOR_ID))
    {
        // if not found, print a warning log message
        printf("PIIX3 not found.\n");

        // return apic initialization failed
        return false;
    }

    // enable APIC
    if (! piix3_ioapic_enable(bus, dev, fun))
    {
        // in case of failure, print a warning log message
        printf("IOAPIC not enabled\n");

        // return apic initialization failed
        return false;
    }

    // disable 8259 controller
    disable_8259();

    // return apic initialization succeded
    return true;
}

