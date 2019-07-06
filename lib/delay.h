/**
 * File: delay.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 *
 */
const ioaddr iCWR_0 = 0x43;

/**
 *
 */
const ioaddr iCTR0_LSB = 0x40;

/**
 *
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
 *
 */
extern "C" void a_delay();               // indirizzo da porre nel gate n. 240

/**
 *
 */
extern "C" void a_driver_timer();        // indirizzo da porre nel gate  n.40

/**
 *
 */
extern void ini_ip_cont_0();
