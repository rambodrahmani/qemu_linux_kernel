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

    // loop through the video array and set each position:
    // use the given color and attribute = ' '
    for (unsigned int i = 0; i < VIDEO_SIZE; i++) 
    {
        video[i] = ' ' | attr << 8;
    }

    // cursor to row 0 column 0
    x = 0;
    y = 0;

    // update the cursor position
    cursor();
}

