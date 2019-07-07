/**
 * File: pci_next.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

bool pci_next(natb& bus, natb& dev, natb& fun)
{
	fun++;

	if (fun < 8)
		return true;

	fun = 0;
	dev++;

	if (dev < 32)
		return true;

	if (bus == 255)
		return false;

	dev = 0;
	bus++;
	
	return true;
}
