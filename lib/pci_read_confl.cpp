/**
 * File: pci_read_confl.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

natl pci_read_confl(natb bus, natb dev, natb fun, natb off)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	natl ret;
	inputl(PCI_CDP, ret);

	return ret;
}
