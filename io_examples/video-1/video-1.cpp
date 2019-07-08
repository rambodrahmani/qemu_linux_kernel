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
    // loop through the video array
    for(int i = 0; i < 2000; i++)
    {
        // 0x4B = 0100 1011
        // set all positions to red background, light blue text, containing 'a'
        video[i] = 0x4B00 | 'a';
    }
    
    // wait for the ESC char
    char c;
    for (;;)
    {
        c = char_read();
        if (c == 0x1B)
        {
            // break and exit if ESC pressed
            break;
        }
    }

    // return
    return 0;
}

/*
 * The color byte is madre up of 3 parts:
 *   0-3: text foreground color;
 *   4-6: background color;
 *     7: blinking bit.
 * The colors are encoded using IRGB (Intensity, Red, Green, Blue) as shown
 * below:
 *
 * Color            IRGB
 * Black            0000
 * Blue             0001
 * Green            0010
 * Azure            0011
 * Red              0100
 * Violet           0101
 * Brown            0110
 * Dark Gray        0111
 * Light Gray       1000
 * Light Blue       1001
 * Light Green      1010
 * Light Azure      1011
 * Light Red        1100
 * Light Violet     1101
 * Yellow           1110
 * White            1111
 *
 * for the text foreground color all the colors can be used, for the background
 * color only the first 8 colors are available (those encoded only using 3 bits
 * and having intensity 0).
 */

