#include "internal.h"
#include "pci.h"

void pci_write_confw(natb bus, natb dev, natb fun, natb off, natw data)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	outputw(data, PCI_CDP + (off & 0x03));
}
