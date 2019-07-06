/**
 * File: natb_conv.cpp
 *       Converts the given natural number n, having the length of a byte, in a
 *       string of 2 chars which represent the given hex digits. Copies the
 *       resulting string into dst.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natb_conv(natb n, char dst[])
{
    return convnat(n, dst, 2);
}

