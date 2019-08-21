/**
 * File: pci_find_dev.cpp
 *       Looks for the PCI device specified using the deviceID and vendorID.
 *       If found, returns true and the PCI device bus, device and function
 *       numbers are passed by reference.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

bool pci_find_dev(natb& bus, natb& dev, natb& fun, natw deviceID, natw vendorID)
{
    // loop through available PCI devices
    // (255 buses with 32 devices each with 8 functions)
    do {
        natw work;

        if ( (work = pci_read_confw(bus, dev, fun, 0)) == 0xFFFF )
        {
            continue;
        }

        // check vendor and device IDs
        if ( work == vendorID && pci_read_confw(bus, dev, fun, 2) == deviceID) 
        {
            // PCI device found
            return true;
        }
    } while (pci_next(bus, dev, fun));

    // PCI device not found
    return false;
}

