/**
 * File: pci_write_confl.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

void pci_write_confl(natb bus, natb dev, natb fun, natb off, natl data)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	outputl(data, PCI_CDP);
}
