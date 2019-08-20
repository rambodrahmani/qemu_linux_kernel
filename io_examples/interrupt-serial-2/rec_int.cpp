/**
 * File: send_int.cpp
 *       In this I/O example we will show how to use the serial interface using
 *       the interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/08/2019.
 */

#include <libqlk.h>

/**
 * Buffer where to store the message received on serial interface COM1.
 */
char buffer[80];

/**
 * Primitive to receive a message of length nn from serial interface COM1 and
 * store it in the memory location addressed by vv[].
 */
extern "C" void serial_receive(natl nn, char vv[]);

/**
 * Developer harness test shows
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    char c;

    // number of chars to receive on serial interface COM1
    natl length;
    length = 25;

    // receive message on serial interface COM1
    serial_receive(length, buffer);

    // write message received on serial interface COM1 to the video output
    for (int i = 0; i < length; i++)
    {
        // write ASCII char to the video output
        char_write(buffer[i]);
    }

    // write a new line to the video output
    char_write('\n');

    // print message to the video output and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

