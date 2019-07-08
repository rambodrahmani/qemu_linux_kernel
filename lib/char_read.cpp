/**
 * File: char_read.cpp
 *       Implementation for the char_read() function.
 *       Returns the ASCII char corresponding to the key pressed on the
 *       keyboard, or 0 if the key is not a valid ASCII char. The LEFT SHIFT key
 *       is taken into account when retrieving the corresponding ASCII char.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

char char_read()
{
    // the keycode received from the keyboard
    natb k;

    // corresponding ASCII char
    char c;

    do
    {
        // retrieve keycode from the keyboard
        k = get_code();
        
        // check left shift make code
        if (k == 0x2A)
        {
            // shift pressed
            shift = true;
        }
        // check left shift break code
        else if (k == 0xAA)
        {
            // shift released
            shift = false;
        }
    } while (k >= 0x80 || k == 0x2A);   // wait for a valid key

    // convert the retrieved keycode to an ASCII char
    // keycode_to_ascii might return 0
    c = keycode_to_ascii(k);

    // return the ASCII char
    return c;
}

