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
 * COM1 RBR (Receiver Buffer Register) I/O address.
 */
const ioaddr iRBR = 0x03F8;

/**
 * COM1 THR (Transmitter Holding Register) I/O address.
 */
const ioaddr iTHR = 0x03F8;

/**
 * COM1 LSR (Line Status Register) I/O address.
 */
const ioaddr iLSR = 0x03FD;

/**
 * COM1 LCR (Line Control Register) I/O address.
 */
const ioaddr iLCR = 0x03FB;

/**
 * COM1 DLR (Divisor Latch Register) LSB I/O address.
 */
const ioaddr iDLR_LSB = 0x03F8;

/**
 * COM1 DLR (Divisor Latch Register) MSB I/O address.
 */
const ioaddr iDLR_MSB = 0x03F9;

/**
 * COM1 MSR (Modem Control Register) I/O address.
 */
const ioaddr iMCR = 0x03FC;

/**
 * COM1 IER (Interrupt Enable Register) I/O address.
 */
const ioaddr iIER = 0x03F9;

/**
 * COM1 IIR (Interrupt Identification Register) I/O address.
 */
const ioaddr iIIR = 0x03FA;

