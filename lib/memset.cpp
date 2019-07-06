/**
 * File: memset.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

void *memset(void *dest, int c, size_t n)
{
    char *dest_ptr = static_cast<char*>(dest);

    for (size_t i = 0; i < n; i++)
    {
        dest_ptr[i] = static_cast<char>(c);
    }
    
    return dest;
}

