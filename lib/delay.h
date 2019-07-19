/**
 * File: delay.h
 *       8254 PROGRAMMABLE INTERVAL TIMER
 *       The Programmable Interval Timer (PIT) chip (also called an 8253/8254
 *       chip) basically consists of an oscillator, a prescaler and 3 
 *       independent frequency dividers. Each frequency divider has an output,
 *       which is used to allow the timer to control external circuitry (for
 *       example, IRQ 0).
 *
 *       The PIT chip has three separate frequency dividers (or 3 separate
 *       channels) that are programmable, in that the value of the "reset
 *       counter" is set by software (the OS). Software also specifies an
 *       action to be taken when the counter reaches zero on each individual
 *       channel. In this way, each channel can be used in one of several
 *       "modes" - for example, as a frequency divider (where the count is
 *       automatically reset) or as a "one shot" timer (where the count isn't
 *       automatically reset).
 *
 *       I/O Ports
 *       The PIT chip uses the following I/O ports:
 *       I/O port     Usage
 *       0x40         Channel 0 data port (read/write)
 *       0x41         Channel 1 data port (read/write)
 *       0x42         Channel 2 data port (read/write)
 *       0x43         Mode/Command register (write only, a read is ignored)
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 * Mode/Command register I/O port address.
 */
const ioaddr iCWR_0 = 0x43;

/**
 * Channel 0 data I/O port address (LSB).
 */
const ioaddr iCTR0_LSB = 0x40;

/**
 * Channel 0 data I/O port address (MSB).
 */
const ioaddr iCTR0_MSB = 0x40;

/**
 * 
 */
const natl sincr = 0;                    // semaforo numero 0

/**
 *
 */
extern natl num_t;

/**
 * Interrupt N. 240 Driver.
 */
extern "C" void a_delay();

/**
 * Interrupt N. 40 Driver.
 */
extern "C" void a_driver_timer();

/**
 * Initializes counter 0 in mode 3.
 */
extern void ini_ip_cont_0();

/*
 * The Mode/Command register at I/O address 0x43 contains the following:
 *
 * Bits         Usage
 *  6 and 7     Select channel:
 *               0 0 = Channel 0
 *               0 1 = Channel 1
 *               1 0 = Channel 2
 *               1 1 = Read-back command (8254 only)
 *
 *  4 and 5     Access mode:
 *               0 0 = Latch count value command
 *               0 1 = Access mode: lobyte only
 *               1 0 = Access mode: hibyte only
 *               1 1 = Access mode: lobyte/hibyte
 *
 *  1 to 3      Operating mode:
 *               0 0 0 = Mode 0 (interrupt on terminal count)
 *               0 0 1 = Mode 1 (hardware re-triggerable one-shot)
 *               0 1 0 = Mode 2 (rate generator)
 *               0 1 1 = Mode 3 (square wave generator)
 *               1 0 0 = Mode 4 (software triggered strobe)
 *               1 0 1 = Mode 5 (hardware triggered strobe)
 *               1 1 0 = Mode 2 (rate generator, same as 010b)
 *               1 1 1 = Mode 3 (square wave generator, same as 011b)
 *
 *  0           BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
 */

