/**
 * File: convnat.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

static const char hex_map[16] =
{
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'
};

char* convnat(natl l, char* out, int cifre)
{
    for(int i = cifre - 1; i >= 0; --i)
    {
        out[i] = hex_map[l % 16];
        l /= 16;
    }

    out[cifre] = '\0';

    return out + cifre;
}

