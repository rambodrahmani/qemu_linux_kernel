/**
 * File: natq_conv.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natq_conv(natq n, char dst[])
{
    return convnat(n, dst, 16);
}

