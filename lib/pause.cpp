/**
 * File: pause.cpp
 *       Prints the string "Press ESC to continue." to the video output and
 *       waits for the ESC key to be pressed.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "kbd.h"

void pause()
{
    // print message to the output video
	str_write("Press ESC to continue.\n");

    // wait for the ESC key
	while (char_read() != 0x1B)
		;
}

