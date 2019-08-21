/**
 * File: pci_next.cpp
 *       Finds the next PCI device starting from the given bus, device and
 *       function number, if available.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

bool pci_next(natb& bus, natb& dev, natb& fun)
{
    // increase function number
    fun++;

    // check if max function number was reached
    if (fun < 8)
    {
        // a new PCI device is available
        return true;
    }

    // reset function number
    fun = 0;

    // increase device number
    dev++;

    // check if max device number was reached
    if (dev < 32)
    {
        // a new PCI device is available
        return true;
    }

    // check if maximum bus number was reached
    if (bus == 255)
    {
        // no more PCI devices are available
        return false;
    }

    // reset device number
    dev = 0;

    // a new PCI device is available on the next bus
    bus++;

    // return true
    return true;
}

