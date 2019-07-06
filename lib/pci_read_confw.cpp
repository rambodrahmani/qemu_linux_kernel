#include "internal.h"
#include "pci.h"

natw pci_read_confw(natb bus, natb dev, natb fun, natb off)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	natw ret;
	inputw(PCI_CDP + (off & 0x03), ret);
	return ret;
}
