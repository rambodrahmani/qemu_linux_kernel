/**
 * File: keyboard_reset.cpp
 *       Implementation for the keyboard_reset() function.
 *       Resets the keyboard do initialization state.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

void keyboard_reset()
{
    // command 60
    outputb(0x60, iCMR);

    // initialize keyboard with interrupts enabled
    outputb(0x61, iTBR);
}

