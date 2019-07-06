/**
 * File: copy.cpp
 *       Copies the source string to the destination string.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* copy(const char* src, char* dst)
{
    while (*src)
    {
        *dst++ = *src++;
    }

    *dst = '\0';

    return dst;
}

