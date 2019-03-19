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
 * Utility functions to read and write in a registry of the I/O space.
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

