/**
 * File: clear_screen.cpp
 *       Implementation for the clear_screen() function.
 *       Clears the screen to the given color value.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "vid.h"

void clear_screen(natb color) 
{
    attr = color;

    for (unsigned int i = 0; i < VIDEO_SIZE; i++) 
    {
        video[i] = ' ' | attr << 8;
    }

    cursor();
}

