/**
 * File: get_code.cpp
 *       Implementation for the get_code() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/03/2019.
 */

#include "internal.h"
#include "kbd.h"

natb get_code()
{
    // return key code
    natb c;

    // wait for a new key to be pressed
    do {
        inputb(iSTR, c);
    } while (!(c & 0x01));

    // read the key code from the keyboard RBR register
    inputb(iRBR, c);

    // return
    return c;
}

