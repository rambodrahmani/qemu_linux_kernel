/**
 * File: str_write.cpp
 *       Writes the given string to the video output (the termination char '\0'
 *       is not included).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

void str_write(const char buf[])
{
    // buffer index
    natl i = 0;

    // loop through the buffer elements
    while (buf[i])
    {
        // call the char_write() function for each letter in the buffer
        char_write(buf[i]);

        // increment the buffer index
        i++;
    }
}

