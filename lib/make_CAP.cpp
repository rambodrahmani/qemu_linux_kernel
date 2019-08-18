/**
 * File: make_CAP.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

natl make_CAP(natb bus, natb dev, natb fun, natb off)
{
	return 0x80000000 | (bus << 16) | (dev << 11) | (fun << 8) | (off & 0xFC);
}

