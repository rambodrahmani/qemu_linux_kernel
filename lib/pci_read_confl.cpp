/**
 * File: pci_read_confl.cpp
 *       Reads a long from the CDP register having internal offset regn relative
 *       to the PCI function specified by fun of the device specified by dev.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

natl pci_read_confl(natb bus, natb dev, natb fun, natb off)
{
    natl ret;

    natl confaddr = make_CAP(bus, dev, fun, off);
    outputl(confaddr, PCI_CAP);
    inputl(PCI_CDP, ret);

    return ret;
}

