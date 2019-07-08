/**
 * File: video.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "vid.h"

/**
 * The video memory can be seen as an array of words visible starting from the
 * physical address 0xB8000: the position indexed 0,0 (top left corner) has
 * index 0 in the array, while the last position (bottom right corner) has index
 * 1999.
 */
natw* video = reinterpret_cast<natw*>(0x000B8000);

