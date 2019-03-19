/**
 *
 * File: lib.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019
 *
 */

#include "type.h"

/*
 * Utility functions to read and write to a registry of the I/O space.
 */

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputb(ioaddr reg, natb &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputb(natb a, ioaddr reg);

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputw(ioaddr reg, natw &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputw(natw a, ioaddr reg);

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputl(ioaddr reg, natl &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputl(natl a, ioaddr reg);

/**
 *
 * @param   vect[]
 * @param   length
 * @param   reg
 *
 */
extern "C" void outputbw(natw vect[], int length, ioaddr reg);

/**
 *
 * @param   reg
 * @param   vect[]
 * @param   length
 *
 */
extern "C" void inputbw(ioaddr reg, natw vect[], int length);

/*
 * Utility functions to read from the keyboard.
 */

/**
 * @return 
 */
natb get_code();

/**
 * @return 
 */
char char_read();

/**
 *
 * @param   c
 *
 * @return
 */
char conv(natb c);

/*
 * Utility functions to setup the video ourput.
 */

/**
 *
 * @param   screen_max_x
 * @param   screen_max_y
 *
 */
natb * bochsvga_config(natw screen_max_x, natw screen_max_y);

/*
 * Utility functions to write to the video output.
 */

/**
 *
 * @param   col
 *
 */
void clear_screen(natb col);

/**
 *
 * @param c
 *
 */
void char_write(natb c);

/**
 *
 * @param str[]
 *
 */
void str_write(const char str[]);

/*
 * Utility functions to read and write to the primary hard disk drive.
 */

/**
 *
 * @param   lba
 * @param   length
 * @param   vect[]
 *
 */
void hdd_read(natl lba, natb length, natw vect[]);

/**
 *
 * @param   lba
 * @param   length
 * @param   vect[]
 *
 */
void hdd_write(natl lba, natb length, natw vect[]);

