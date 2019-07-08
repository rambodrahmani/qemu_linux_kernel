/**
 * File: video-1.cpp
 *       In text mode, the display is devided into 25 rows and 80 columns, for a
 *       total of 2000 positions. To each position is associated a word in the
 *       video memory, which is made up of the color byte (the most significant
 *       one) and an information byte (the less significant one).
 *
 *       The color byte contains the background color and the text foreground
 *       color. The information byte contains the ASCII code for a char.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libqlk.h>
#include <vid.h>

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
    natb c;
    
    // red background color, azure foregound color
    clear_screen(0x4B);

    // read ASCII chars from the keyboard
    for (;;) 
    {
        c = char_read();
        
        // check for the ESC key
        if (c == 0x1B)
        {
            break;
        }

        // write the char to the output video
        char_write(c);
	}

    return 0;
}

