/**
 * File: svga-1.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 12/07/2019.
 */

#include <libqlk.h>

const natl COLS = 1280;
const natl ROWS = 1024;

natb* framebuffer;

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
    // initialize the framebuffer with the given resolution
    framebuffer = bochsvga_config(COLS, ROWS);

    // set the framebuffer to dark read
    for (int i=0; i<COLS; i++)
    {
        for (int j=0; j<ROWS; j++)
        {
            framebuffer[j*COLS + i] = 0x04;
        }
    }

    // wait for te ESC char
    char c;
    do
    {
        c = char_read();
    } while (c != 0x1B);
    
    // return
    return 0;
}

