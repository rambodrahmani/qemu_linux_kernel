/**
 * File: tim_car.cpp
 *       This I/O example shows how to use the 8254 programmable interval timer
 *       interface using the interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 18/08/2019.
 */

#include <libqlk.h>

/**
 * Primitive type 242.
 */
extern "C" void go_timer(natl num);

/**
 * Developer harness test shows how to use the 8254 with the interrupt mechanism
 * to print to the video output a starting line containing the letter 'A', a
 * second line containing a sequence of N 'D' chars printed with intervals of
 * 50ms and a final line containing the 'Z' char.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // number of 'D' chars to be printed on the second line
    natl nn;
    nn = 60;

    // write 'A' to the video output
    char_write('A');

    // write a new line to the video output
    char_write('\n');

    // call the go_timer() primitive passing the number of 'D' chars
    go_timer(nn);

    // write a new line to the video output
    char_write('\n');

    // write 'Z' to the video output
    char_write('Z');

    // write a new line to the video output
    char_write('\n');

    // print pause message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

