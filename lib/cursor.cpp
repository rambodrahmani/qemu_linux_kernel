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

    outputb(CUR_HIGH, iIND);
    outputb((natb)(pos >> 8), iDAT);
    outputb(CUR_LOW, iIND);
    outputb((natb)pos, iDAT);
}

