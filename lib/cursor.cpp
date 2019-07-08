/**
 * File: cursor.cpp
 *       Implementation for the cursor() function.
 *       Adjusts the cursor position on the video outout.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "libqlk.h"
#include "vid.h"

void cursor()
{
    // new cursor position
    natw pos = COLS * y + x;

    // write new position to the cursor high register
    outputb(CUR_HIGH, ADDR);
    outputb((natb)(pos >> 8), DATA);

    // write new position to the cursor low register
    outputb(CUR_LOW, ADDR);
    outputb((natb)pos, DATA);
}

