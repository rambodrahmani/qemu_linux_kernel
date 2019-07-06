/**
 * File: natl_conv.cpp
 *       Converts the given natural number n, having the length of a long, in a
 *       string of 8 chars which represent the given hex digits. Copies the
 *       resulting string into dst.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natl_conv(natl n, char dst[])
{
    return convnat(n, dst, 8);
}

