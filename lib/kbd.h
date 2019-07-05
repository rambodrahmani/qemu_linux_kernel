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
 * Keyboard RBR register address.
 */
const ioaddr iRBR = 0x60;

/**
 * Keyboard TBR register address.
 */
const ioaddr iTBR = 0x60;

/**
 * Keyboard CMR register address.
 */
const ioaddr iCMR = 0x64;

/**
 * Keyboard STR register address.
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
 * Initializes the keyboard.
 */
void keyboard_init();

/**
 * Resets the keyboard.
 */
void keyboard_reset();

/**
 * Clears the keyboard RBR register by reading any available char.
 */
void keyboard_clear();

/**
 * Reads the key pressed on the keyboard and returns the corresponding ASCII
 * char.
 *
 * @return  the ASCII char corresponding to the keycode received from the
 *          keyboard
 */
natb keyboard_read();

