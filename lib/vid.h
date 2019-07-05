/**
 * File: vid.h
 *       This file contains video related constants and functions.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/07/2019.
 */

#include "type.h"

/**
 *
 */
const natl COLS = 80;

/**
 *
 */
const natl ROWS = 25;

/**
 *
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
 *
 */
const ioaddr iIND = 0x03D4;

/**
 *
 */
const ioaddr iDAT = 0x03D5;


/**
 *
 */
extern natw* video;

/**
 *
 */
extern natb x, y;

/**
 *
 */
extern natw attr;


/**
 * Adjusts the cursor position on the video outout.
 */
void cursor();

/**
 *
 */
void scroll();

