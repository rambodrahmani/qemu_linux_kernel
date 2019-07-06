/**
 * File: natq_conv.cpp
 *       Converts the given natural number n, having the length of a quad, in a
 *       string of 16 chars which represent the given hex digits. Copies the
 *       resulting string into dst.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* natq_conv(natq n, char dst[])
{
    return convnat(n, dst, 16);
}

