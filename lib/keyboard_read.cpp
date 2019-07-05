/**
 * File: keyboard_read.cpp
 *       Implementation for the keyboard_read() function.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "internal.h"

natb keyboard_read()
{
	natb a = char_read();

	char_write(a);

	return a;
}

