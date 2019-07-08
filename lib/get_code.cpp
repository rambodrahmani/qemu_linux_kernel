/**
 * File: get_code.cpp
 *       Implementation for the get_code() function.
 *       Returns the keycode of the key presssed or released on the keyboard.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/03/2019.
 */

#include "internal.h"
#include "kbd.h"

natb get_code()
{
    // keycode byte
    natb k;

    // check the status register: wait for a new key to be pressed
    do {
        inputb(iSTR, k);
    } while (!(k & 0x01));

    // read the keycode from the keyboard RBR register
    inputb(iRBR, k);

    // return the keycode
    return k;
}

