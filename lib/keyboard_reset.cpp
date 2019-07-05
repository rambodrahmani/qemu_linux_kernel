/**
 * File: keyboard_reset.cpp
 *       Implementation for the keyboard_reset() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

void keyboard_reset()
{
	outputb(0x60, iCMR);
	outputb(0x61, iRBR);
}

