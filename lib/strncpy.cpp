/**
 * File: strncpy.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

char *strncpy(char *dest, const char *src, natl l)
{
        size_t i;

        for(i = 0; i < l && src[i]; ++i)
                dest[i] = src[i];

        return dest;
}
