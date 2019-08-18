/**
 * File: tim_notes.cpp
 *       In this I/O example we will show how to associate notes to the keyboard
 *       keys and create a small piano with 7 notes available.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 18/08/2019.
 */

#include <libqlk.h>

/**
 * Keyboard keys associated to the available notes.
 */
const natb keys[] = { 'q', 'w', 'e', 'r', 't', 'y', 'u' };

/**
 * Time constants associated with each note. [1]
 */
const natw ctr[] = { 4548, 4052, 3610, 3408, 3036, 2704, 2410 };

/**
 * Returns the time constant corresponding to the given key.
 */
natw cost(natb key)
{
    // return value
    natw c = 0;

    // loop through the available keys
    for (int i = 0; i < 7; i++)
    {
        if (keys[i] == key)
        {
            c = ctr[i];
            break;
        }
    }

    // return
    return c;
}

/**
 *
 */
extern "C" void note(natw cc);

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard and
 * set the frequency of the sound produced by the speaker. [0]
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // loop until the ESC key is pressed
    for (;;)
    {
        // wait for a char from the keyboard
        natb t = char_read();

        // check if the ESC char was retrieved
        if (t == 0x1B)
        {
            // exit loop
            break;
        }

        // retrieve the frequency corresponding to the retrieved char
        natw c = cost(t);

        // if a valid frequency is retrieved
        if (c != 0)
        {
            // call primitive note() with the retrieved time constant
            note(c);
        }
    }

    // return with no errors
    return 0;
}

/**
 * [0]
 * The output of PIT channel 2 is connected to the PC speaker, so the frequency
 * of the output determines the frequency of the sound produced by the speaker.
 */

/**
 * [1]
 * Frequencies and time constants for the 7 notes:
 * 
 *    Note    Frequency    Time Constant
 *     DO       261.63         4548
 *     RE       293.66         4052
 *     MI       329.63         3610
 *     FA       349.23         3408
 *     SOL      392.00         3036
 *     LA       440.00         2704
 *     SI       493.88         2410
 */

