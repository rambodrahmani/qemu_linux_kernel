/**
 * File: send.cpp
 *       This I/O example shows how to send data using the serial interface
 *       COM1.
 *
 *       This example must be run in combination with serial-2/receive.cpp: QEMU
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
    // clear screen background to blue
    clear_screen(0x17);

    // initialize serial interface COM1
    init_COM1();

    for (;;) 
    {
        // read an ASCII char from the keyboard
        char c = char_read();

        // write the retrieved ASCII char to the COM1 THR register
        serial1_o(c);

        // check for ESC char
        if (c == 0x1B)
        {
            // break if c = ESC char
            break;
        }

        // write the retrived ASCII char to the video output
        char_write(c);
    }

    // return
    return 0;
}

