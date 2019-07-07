/**
 * File: serial2_o.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM2.h"

void serial2_o(natb c)
{
	natb s;

	do
    {
		inputb(iLSR, s);
	} while (! (s & 0x20));

	outputb(c, iTHR);
}

