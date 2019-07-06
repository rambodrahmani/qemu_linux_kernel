/**
 * File: serial_o.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM1.h"

void serial_o(natb c)
{
	natb s;

	do
    {
		inputb(iLSR, s);
	} while (! (s & 0x20));
	
    outputb(c, iTHR);
}

