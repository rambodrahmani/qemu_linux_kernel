/**
 * File: pci_write_confb.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

void pci_write_confb(natb bus, natb dev, natb fun, natb off, natb data)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	outputb(data, PCI_CDP + (off & 0x03));
}
