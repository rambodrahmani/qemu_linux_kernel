/**
 * File: keycode_to_ascii.cpp
 *       Implementation for the keycode_to_ascii() function.
 *       Converts the given keycode to the corresponding ASCII char.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

char keycode_to_ascii(natb c)
{
	char cc;

	natl pos = 0;
	
    while (pos < MAX_CODE && tab[pos] != c)
    {
		pos++;
    }

    if (pos == MAX_CODE)
    {
        return 0;
    }

    if (shift)
    {
        cc = tabmai[pos];
    }
    else
    {
        cc = tabmin[pos];
    }

    return cc;
}

