/**
 * File: user.cpp
 *       A very simple user program example.
 *
 *       After placing the file in user/user.cpp compile und run using:
 *          make
 *          make swap
 *          ./run
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/09/2019
 */

#include <sys.h>
#include <lib.h>

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
    // write 'Hello world!' to the video output
    writeconsole("Hello world!");

    // write pause message and wait for the ESC key
    pause();

    // terminate user process
    terminate_p();
}

