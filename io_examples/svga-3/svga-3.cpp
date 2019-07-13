/**
 * File: svga-2.cpp
 *       Draws the graph of a rect and of a parable on the VGA video output
 *       using the graphic mode.
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
 * Paints the specified pixel to the specified color.
 *
 * @param   x   pixel x-coordinate
 * @param   y   pixel y-coordinate
 * @param   c   desired color for the pixel
 */
void paint_pixel(int x, int y, natb c)
{
    framebuffer[COLS*y + x] = c;
}

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
    framebuffer = bochsvga_config(COLS,ROWS);

    int x, y, xc, yc;

    // paint the background of the framebuffer to yellow
    background(0x36);

    // draw dark blue axis
    axis(0x01);

    // draw yc = xc/2 - 100
    for (x=0; x<COLS; x++)
    {
        xc = x-x0;
        yc = xc/2 - 100;
        y  = y0-yc;

        if (y >= 0 && y < ROWS)
        {
            paint_pixel(x, y, 0x02);
        }
    }

    // parable yc = xc*xc/200 - xc/2 - 100
    for (x=0; x<COLS; x++)
    {
        xc = x-x0;
        yc = xc*xc/200 - xc*2 - 100;
        y = y0 - yc;
        if (y >= 0 && y < ROWS)
        {
            paint_pixel(x, y, 0x04);
        }
    }

    // wait for the ESC char
    char c;
    do 
    {
        c = char_read();
    } while (c != 0x1B);

    // return
    return 0;
}

