/**
 * File: interrupt-keyboard.cpp
 *       This I/O example shows how to use the keyboard 8042 interface with the
 *       interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>

/**
 * Keyboard ASCII chars buffer.
 */
char buffer[80];

/**
 * This primitive invokes interrupt 241. See read_line.s.
 *
 * @param   &len     number of chars retrieved from keyboard (max 80)
 * @param   buff[]   buffer containing the retrieved chars
 */
extern "C" void read_line(int& len, char buff[]);

/**
 * Developer harness test.
 *
 * After the call to the read_line primitive, length will contain the number of
 * ASCII chars retrieved from keyboard inputs and buffer will contain the actual
 * ASCII chars.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    int length;

    // call read_line primitive
    read_line(length, buffer);

    // print ASCII chars buffer to the video output
    for (int i = 0; i < length; i++)
    {
        char_write(buffer[i]);
    }

    // print pause message and wait for the ESC key
    pause();

    return 0;
}

