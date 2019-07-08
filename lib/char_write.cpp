/**
 * File: char_write.cpp
 *       Implementation for the char_write() function.
 *       Writes the given char to the video output.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "vid.h"

void char_write(natb c)
{
    // switch case on the given ASCII char
    switch (c)
    {
        case 0:
        {
            break;
        }

        // ASCII CR: go to column 0 on the same line
        case '\r':
        {
            x = 0;
            break;
        }

        // ASCII LF: go to column 0 on the following line
        case '\n':
        {
            // go to column 0
            x = 0;
            
            // increase line
            y++;

            // scroll down if the maximum number of rows is reached
            if (y >= ROWS)
            {
                scroll();
            }

            break;
        }

        case '\b':
        {
            if (x > 0 || y > 0) 
            {
                if (x == 0)
                {
                    x = COLS - 1;
                    y--;
                }
                else
                {
                    x--;
                }
            }
            break;
        }

        // ASCII char
        default:
        {
            // set color and text attribute for the given position
            video[y * COLS + x] = c | attr << 8;

            // increase cursor position
            x++;

            // go to the following line if the maximum number of column is
            // reached
            if (x >= COLS)
            {
                x = 0;
                y++;
            }

            // scroll down if the maximum number of rows is reached
            if (y >= ROWS) 
            {
                scroll();
            }
            
            break;
        }
    }

    // update cursor position
    cursor();
}

