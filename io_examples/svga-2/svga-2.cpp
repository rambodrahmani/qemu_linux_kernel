/**
 * File: svga-2.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 12/07/2019.
 */

#include <libqlk.h>

// framebuffer resolution
const natl COLS = 1280;
const natl ROWS = 1024;

// screen center coordinates
const int x0 = (COLS-1)/2;
const int y0 = (ROWS-1)/2;

// video card framebuffer
natb* framebuffer;

/**
 * Paints the framebuffer background color to the given color c.
 */
void background(natb c)
{
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            framebuffer[j*COLS + i] = c;
        }
    }
}

/**
 * Draws a rectangular border of the specified color on the framebuffer.
 */
void border(natb c)
{
    // top border
    for (int i = 0; i < COLS; i++)
    {
        framebuffer[i] = c;
    }

    // right border
    for (int j = 0; j < ROWS; j++)
    {
        framebuffer[COLS*j+(COLS-1)] = c;
    }

    // bottom border
    for (int i = 0; i < COLS; i++)
    {
        framebuffer[COLS*(ROWS-1)+i] = c;
    }

    // left border
    for (int j = 0; j < ROWS; j++)
    {
        framebuffer[COLS*j] = c;
    }
}

/**
 * Draws axis of the specified color on the framebuffer.
 */
void axis(natb c)
{
    // x axis
    for (int i = 0; i < COLS; i++)
    {
        framebuffer[COLS*y0 + i] = c;
    }

    // y axis
    for (int j = 0; j < ROWS; j++)
    {
        framebuffer[COLS*j + x0] = c;
    }
}

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
    // initialize the video card framebuffer
    framebuffer = bochsvga_config(COLS, ROWS);

    // paint the background of the framebuffer to yellow
    background(0x36);
    
    // draw a dark red border
    border(0x04);
    
    // draw dark blue axis
    axis(0x01);
    
    // wait for the ESC char
    char c;
    do
    {
        c = char_read();
    } while (c != 0x1B);

    // return
	return 0;
}

