/**
 * File: interrupt-keyboard.cpp
 *       This I/O example shows how to use the keyboard 8042 interface with the
 *       interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>

char buff_tas[80];

/**
 *
 */
extern "C" void read_line(int& nn, char vv[]);

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
    char c;

    int quanti;

    read_line(quanti, buff_tas);

    // ...

    for (int i = 0; i < quanti; i++)
    {
        char_write(buff_tas[i]);
    }

    // print pause message and wait for the ESC key
    pause();

    return 0;
}

