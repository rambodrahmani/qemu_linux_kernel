/**
 * File: kbd.h
 *       This file contains keyboard related constants and functions.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/03/2019.
 */

/**
 *
 */
const natl MAX_CODE = 30;

/**
 *
 */
const ioaddr iRBR = 0x60;

/**
 *
 */
const ioaddr iTBR = 0x60;

/**
 *
 */
const ioaddr iCMR = 0x64;

/**
 *
 */
const ioaddr iSTR = 0x64;

/**
 *
 */
extern bool shift;

/**
 *
 */
extern natb tab[MAX_CODE];

/**
 *
 */
extern char tabmin[MAX_CODE];

/**
 *
 */
extern char tabmai[MAX_CODE];

/**
 *
 */
void keyboard_init();

/**
 *
 */
void keyboard_reset();

/**
 *
 */
void clear();

/**
 *
 */
natb read();

