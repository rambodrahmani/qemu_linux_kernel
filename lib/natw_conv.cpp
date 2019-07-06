/**
 * File: natw_conv.cpp
 *       Converts the given natural number n, having the length of a word, in a
 *       string of 4 chars which represent the given hex digits. Copies the
 *       resulting string into dst.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natw_conv(natw n, char dst[])
{
    return convnat(n, dst, 4);
}

