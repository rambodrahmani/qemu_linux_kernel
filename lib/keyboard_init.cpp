/**
 * File: keyboard_init.cpp
 *       Implementation for the keyboard_init() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/03/2019.
 */

#include "internal.h"
#include "kbd.h"

void keyboard_init()
{
	outputb(0x60, iCMR);
	outputb(0x60, iRBR);
}

