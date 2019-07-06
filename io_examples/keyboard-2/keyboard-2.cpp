/**
 * File: keyboard-2.cpp
 *       Keep in mind that each keyboard key has one and only one make code and
 *       one and only one break code. However, to the same keyboard key can
 *       correspond more than one ASCII char, based on some special keys
 *       (e.g. SHIFT, ALT, CTRL) combinations (if pressed or not).
 *
 *       The following program takes into account the 26 keys on the keyboard
 *       corresponding to letters and prints to video the lowercase or uppercase
 *       of the pressed letter based on the value of the LEFT SHIFT key.
 *       All other keys won't have any effect, except for the ESC key which will
 *       terminate the program, the ENTER key ('\n') and the SPACE BAR key
 *       (' ').
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libcqlk.h>
#include <kbd.h>

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard and
 * print their binary representation the video output.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    char c;

    for (;;)
    {
        // get the ascii char corresponding to the pressed key
        c = char_read();

        // che if the ESC char was retrieved
        if (c == 0x1B)
        {
            break;
        }

        // print the given ASCII char to the video output
        char_write(c);
    }

    // return with no errors
    return 0;
}

