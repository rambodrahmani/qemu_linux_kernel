/**
 * File: pci_find_class.cpp
 *       Finds and returns (by reference) the class code for a given function of
 *       a given device on a given bus. Returns true if the class code is found
 *       and false otherwise.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

bool pci_find_class(natb& bus, natb& dev, natb& fun, natb code[])
{
    do
    {
        if (pci_read_confw(bus, dev, fun, 0) == 0xFFFF)
        {
            continue;
        }

        natb work[3];
        natl i;
        for (i = 0; i < 3; i++)
        {
            work[i] = pci_read_confb(bus, dev, fun, 2 * 4 + i + 1);

            if (code[i] != 0xFF && code[i] != work[i])
            {
                break;
            }
        }

        if (i == 3)
        {
            for (i = 0; i < 3; i++)
            {
                code[i] = work[i];
            }

            return true;
		}
	} while (pci_next(bus, dev, fun));

    return false;
}

