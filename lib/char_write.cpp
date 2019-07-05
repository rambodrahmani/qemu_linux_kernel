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
    switch (c)
    {
        case 0:
        {
            break;
        }

        case '\r':
        {
            x = 0;
            break;
        }

        case '\n':
        {
            x = 0;
            y++;
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

        default:
        {
            video[y * COLS + x] = c | attr << 8;
            x++;
            if (x >= COLS)
            {
                x = 0;
                y++;
            }

            if (y >= ROWS) 
            {
                scroll();
            }
            
            break;
        }
    }

    cursor();
}

