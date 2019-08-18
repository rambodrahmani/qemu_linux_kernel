/**
 * File: mod_notes.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 18/08/2019.
 */

#include <libqlk.h>

/**
 * Speaker Register I/O address.
 */
const ioaddr iSPR = 0x61;

/**
 * Mode/Command register I/O port address.
 */
const ioaddr iCWR = 0x43;

/**
 * Channel 0 data I/O port address (LSB).
 */
const ioaddr iCTR0_LSB = 0x40;

/**
 * Channel 0 data I/O port address (MSB).
 */
const ioaddr iCTR0_MSB = 0x40;

/**
 * Channel 2 data I/O port address (LSB).
 */
const ioaddr iCTR2_LSB = 0x42;

/**
 * Channel 2 data I/O port address (MSB).
 */
const ioaddr iCTR2_MSB = 0x42;

/**
 * Initializes counter 2 in mode 3 without setting a time constant.
 */
void ini_CONT_2()
{
    outputb(0xB6, iCWR);
}

/**
 *
 */
extern "C" void a_note();

/**
 * Initializes custom IDT entries and the APIC pins.
 */
void ini()
{
    // initialize counter 2
    ini_CONT_2();

    // initialize IDT entry 243 with the a_note() address
    gate_init(243, a_note);
}

/**
 * c_note() primitive C++ body implementation. It writes the given time constant
 * for the counter, enables the speaker writing to the SPR register, calls the
 * primitive delay() and disables the speaker by writing to the SPR register.
 *
 * @param  cc   time constant for counter 2 (the speaker frequency).
 */
extern "C" void c_note(natw cc)
{
    // write time constant for counter 2 (LSB)
    outputb(cc, iCTR2_LSB);

    // write time constant for counter 2 (MSB)
    outputb(cc >> 8, iCTR2_MSB);

    // enable speaker
    outputb(3, iSPR);

    // wait
    delay(5);

    // disable speaker
    outputb(0, iSPR);
}

