/**
 * File: keyboard_init.cpp
 *       Implementation for the keyboard_init() function.
 *
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/03/2019.
 */

#include "internal.h"
#include "kbd.h"

void keyboard_init()
{
    // command 60: write 8042 Command Byte: next data byte written to port 60h
    // is placed in 8042 command register.
	outputb(0x60, iCMR);

    // initialize keyboard with no interrupts, 0x60 = 01100000, [1]
	outputb(0x60, iTBR);
}

/* [1]
 * 60   Write 8042 Command Byte: next data byte written to port 60h is
 *      placed in 8042 command register.
 *
 *      Format:
 *	     |7|6|5|4|3|2|1|0|	8042 Command Byte
 *	      | | | | | | | `---- 1=enable output register full interrupt
 *	      | | | | | | `----- should be 0
 *	      | | | | | `------ 1=set status register system, 0=clear
 *	      | | | | `------- 1=override keyboard inhibit, 0=allow inhibit
 *	      | | | `-------- disable keyboard I/O by driving clock line low
 *	      | | `--------- disable auxiliary device, drives clock line low
 *	      | `---------- IBM scancode translation 0=AT, 1=PC/XT
 *	      `----------- reserved, should be 0
 */

