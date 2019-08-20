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
 * Buffer for the chars to be sent on serial interface COM1.
 */
char buffer[80];

/**
 * Primitive to send chars on serial interface COM1.
 */
extern "C" void serial_send(natl nn, char vv[]);

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
    // max 80
    natl length = 25;

    // string to be sent
    char s[] = "I calcolatori elettronici";

    // copy string to be sent to the buffer
    for (int i = 0; i < length; i++)
    {
        buffer[i] = s[i];
    }

    // delay(20); 

    // send the string over serial interface COM1
    serial_send(length, buffer);

    // write string to the video output
    str_write("OK\n");

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

