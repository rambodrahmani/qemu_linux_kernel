/**
 * File: int_conv.cpp
 *       Converts the given integer number n in a string of chars which
 *       represent the  sign and the hex digits. Copies the resulting string
 *       into dst.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

char* int_conv(long n, char* out)
{
    char buf[22];
    
    int i = 11;

    bool neg = false;

    if (n == 0) 
    {
        return copy("0", out);
    }

    buf[i--] = '\0';

    if (n < 0)
    {
        n = -n;
        neg = true;
    }

    while (n > 0)
    {
        buf[i--] = n % 10 + '0';
        n = n / 10;
    }

    if (neg)
    {
        buf[i--] = '-';
    }

    return copy(buf + i + 1, out);
}

