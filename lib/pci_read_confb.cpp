#include "internal.h"
#include "pci.h"

natb pci_read_confb(natb bus, natb dev, natb fun, natb off)
{
	natl confaddr = make_CAP(bus, dev, fun, off);
	outputl(confaddr, PCI_CAP);
	natb ret;
	inputb(PCI_CDP + (off & 0x03), ret);
	return ret;
}
