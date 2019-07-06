/**
 * File: natl_conv.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natl_conv(natl n, char dst[])
{
    return convnat(n, dst, 8);
}

