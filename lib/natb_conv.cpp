/**
 * File: natb_conv.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natb_conv(natb n, char dst[])
{
    return convnat(n, dst, 2);
}

