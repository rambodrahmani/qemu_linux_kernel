/**
 * File: pci_read_confb.cpp
 *       Reads a byte from the register having internal offset regn relative to
 *       the PCI function specified by fun of the device specified by dev.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

natb pci_read_confb(natb bus, natb dev, natb fun, natb off)
{
    natb ret;

    natl confaddr = make_CAP(bus, dev, fun, off);
    outputl(confaddr, PCI_CAP);
    inputb(PCI_CDP + (off & 0x03), ret);

    return ret;
}

