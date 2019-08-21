/**
 * File: strlen.cpp
 *       Calculates and returns the length (number of chars) of the
 *       given string.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

size_t strlen(const char *s)
{
    // size counter
    size_t l = 0;

    // loop through s string chars
    while(*s++)
    {
        // count chars
        ++l;
    }

    // return string size
    return l;
}

