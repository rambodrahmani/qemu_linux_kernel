/**
 * File: scroll.cpp
 *       Implementation for the scroll() function.
 *       Scrolls up the content of the display: when the line number reaches 25,
 *       it copies each char in the previous line starting from the second line
 *       to the last one. The new line available at the bottom of the display is
 *       filled with the SP ASCII char (space) if the attribute does not contain
 *       anything.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "vid.h"

void scroll()
{
    natw work = attr;

    // copy the content of each line in the previous one
    for (unsigned int i = 0; i < VIDEO_SIZE - COLS; i++) 
    {
        video[i] = video[i + COLS];
    }

    // fill the last line with the space char if no attr is available
    for (unsigned int i = 0; i < COLS; i++)
    {
        video[VIDEO_SIZE - COLS + i] = ' ' | work << 8;
    }

    y--;
}

