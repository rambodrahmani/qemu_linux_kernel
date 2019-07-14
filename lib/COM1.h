/**
 * File: COM1.h
 *       The 8250 UART (universal asynchronous receiver/transmitter) is an
 *       integrated circuit designed for implementing the interface for serial
 *       communications. The part was originally manufactured by the National
 *       Semiconductor Corporation. It was commonly used in PCs and related
 *       equipment such as printers or modems. The 8250 included an on-chip
 *       programmable bit rate generator, allowing use for both common and
 *       special-purpose bit rates which could be accurately derived from an
 *       arbitrary crystal oscillator reference frequency.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 *
 */
const ioaddr iRBR = 0x03F8;     // DLAB deve essere 0

/**
 *
 */
const ioaddr iTHR = 0x03F8;     // DLAB deve essere 0

/**
 *
 */
const ioaddr iLSR = 0x03FD;

/**
 *
 */
const ioaddr iLCR = 0x03FB;

/**
 *
 */
const ioaddr iDLR_LSB = 0x03F8;     // DLAB deve essere 1

/**
 *
 */
const ioaddr iDLR_MSB = 0x03F9;     // DLAB deve essere 1

/**
 *
 */
const ioaddr iIER = 0x03F9;     // DLAB deve essere 0

/**
 *
 */
const ioaddr iMCR = 0x03FC;

/**
 *
 */
const ioaddr iIIR = 0x03FA;

