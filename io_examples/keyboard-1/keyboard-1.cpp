/**
 * File: keyboard-1.cpp
 *       This I/O example shows how to use the libqlk library utility functions
 *       to retrieve keycodes from the keyboard and print them to the video
 *       output.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libqlk.h>
#include <kbd.h>

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard using
 * the get_code() function and print their binary representation to the video
 * output using a for loop and a binary shift.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // keycode byte
    natb k;

    // infinite loop: wait for keycodes from the keyboard
    for (;;)
    {
        // retrieve key code
        k = get_code();

        // ESC make code
        if (k == 0x01)
        {
            break;
        }

        // shift and test the most significant bit of the keycode
        for (int i = 0; i < 8; i++)
        {
            // 0x80 = 1000 0000
            if (k & 0x80)
            {
                // print '1'
                char_write('1');
            }
            else
            {
                // print '0'
                char_write('0');
            }

            // shift left the keycode
            k <<= 1;
        }

        // print new line
        char_write('\n');
    }

    // return with no errors
    return 0;
}

