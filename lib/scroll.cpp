/**
 * File: scroll.cpp
 *       Implementation for the scroll() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "vid.h"

void scroll()
{
    natw work = attr;

    for (unsigned int i = 0; i < VIDEO_SIZE - COLS; i++) 
    {
        video[i] = video[i + COLS];
    }

    for (unsigned int i = 0; i < COLS; i++)
    {
        video[VIDEO_SIZE - COLS + i] = ' ' | work << 8;
    }

    y--;
}

