/**
 * File: keycode_to_ascii.cpp
 *       Implementation for the keycode_to_ascii() function.
 *       Converts the given keycode to the corresponding ASCII char.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"
#include "kbd.h"

char keycode_to_ascii(natb k)
{
    // ASCII char
    char c;

    // ASCII char position
    natl pos = 0;
	
    // search for the keycode position in the chars array
    while (pos < MAX_CODE && chars[pos] != k)
    {
		pos++;
    }

    // return 0 if the keycode is not found
    if (pos == MAX_CODE)
    {
        return 0;
    }

    // chec if the shift key was pressed
    if (shift)
    {
        // retrieve ASCII char from uppercase chars
        c = uppercase_chars[pos];
    }
    else
    {
        // retrieve ASCII char from lowercase chars
        c = lowercase_chars[pos];
    }

    // return ASCII char
    return c;
}

