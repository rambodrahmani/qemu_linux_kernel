/**
 * File: receive.cpp
 *       This I/O example shows how to receive data using the serial interface
 *       COM1.
 *
 *       This example must be run in combination with serial-1/send.cpp: QEMU
 *       emulated serial interface COM1 is connected to the standard input and
 *       to the standard output (cin and cout streams), usually the UNIX
 *       terminal. Therefore, the send program will retrieve ASCII chars from
 *       the emulated keyboard and send them to the video output and to the COM1
 *       serial interface (which will echo them to the cout stream as well). The
 *       receive program reads ASCII chars from the COM1 serial interface
 *       (connected to the cin stream) and outputs them to the video output. The
 *       UNIX pipe command must be used to connect the cout stream on the first
 *       QEMU instance (running the send program) to the cin stream of the
 *       second QEMU instance (running the receive program).
 *
 *       Run this example using:
 *          boot serial-1/a.out | boot serial-2/a.out
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 15/07/2019.
 */

#include <internal.h>
#include <COM1.h>

/**
 * Developer harness test.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // initialize serial interface COM1
    init_COM1();

    // infinite loop
    for (;;) 
    {
        // read a byte (ASCII char) from serial interface COM1
        natb c;
        serial1_i(c);

        // check for the ESC char
        if (c == 0x1B)
        {
            // exit foor
            break;
        }

        // write the retrieved ASCII char to the video output
        char_write(c);
    }

    // return
    return 0;
}

