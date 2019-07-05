/**
 * File: keyboard_clear.cpp
 *       Implementation for the keyboard_clear() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

void keyboard_clear()
{
    natb a, c;

    for (;;)
    {
        inputb(iSTR, c);

        if (!(c & 0x01))
        {
            break;
        }

        inputb(iRBR, a);
    }
}

