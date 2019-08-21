/**
 * File: strncpy.cpp
 *       Copies the first num characters of source to destination. If the end of
 *       the source C string (which is signaled by a null-character) is found
 *       before num characters have been copied, destination is padded with
 *       zeros until a total of num characters have been written to it.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

char *strncpy(const char *source, char *destination, natl num)
{
    // for loop index
    size_t i;

    // loop through source string chars
    for(i = 0; i < num && source[i]; ++i)
    {
        // copy to destination string
        destination[i] = source[i];
    }

    // return destination string
    return destination;
}

