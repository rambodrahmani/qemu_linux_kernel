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
