/**
 * File: vid.h
 *       This file contains video related constants and functions.
 *       One of the most used video controllers is VGA (Video Graphics Array).
 *       It is made up of a series of registers which can be accessed using the
 *       ports ADDR and DATA: the ADDR port contains the address of the selected
 *       register, while the DATA port is used to read or write data to the
 *       addressed register. The two ports are mounted in the I/O space and are
 *       placed at addresses 0x03D4 (ADDR) and 0x03D5 (DATA). The video
 *       controller registers (having length of a byte) have consequent
 *       one byte long addresses starting from 0.
 *
 *       The display can be thought as divided into rows and columns. A row
 *       index and a column index ca be used to identify a position: in text
 *       mode to each position is associated a char, while in graphic mode to
 *       each position is associated a pixel. The number of rows and columns
 *       depend on the video mode and are greater in video mode than in text
 *       mode.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "type.h"

/**
 * Display columns number.
 */
const natl COLS = 80;

/**
 * Display rows number.
 */
const natl ROWS = 25;

/**
 * Display matrix size.
 */
const natl VIDEO_SIZE = COLS * ROWS;

/**
 *
 */
const natb CUR_HIGH = 0x0e;

/**
 *
 */
const natb CUR_LOW = 0x0f;


/**
 * ADDR port address.
 */
const ioaddr ADDR = 0x03D4;

/**
 * DATA port address.
 */
const ioaddr DATA = 0x03D5;


/**
 *
 */
extern natw * video;

/**
 *
 */
extern natb x, y;

/**
 *
 */
extern natw attr;


/**
 * Adjusts the cursor position on the video output.
 */
void cursor();

/**
 *
 */
void scroll();

