/**
 * File: COM2.h
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
 * COM2 RBR (Receiver Buffer Register) I/O address.
 */
const ioaddr iRBR = 0x02F8;

/**
 * COM2 THR (Transmitter Holding Register) I/O address.
 */
const ioaddr iTHR = 0x02F8;

/**
 * COM2 LSR (Line Status Register) I/O address.
 */
const ioaddr iLSR = 0x02FD;

/**
 * COM2 LCR (Line Control Register) I/O address.
 */
const ioaddr iLCR = 0x02FB;

/**
 * COM2 DLR (Divisor Latch Register) LSB I/O address.
 */
const ioaddr iDLR_LSB = 0x02F8;

/**
 * COM2 DLR (Divisor Latch Register) MSB I/O address.
 */
const ioaddr iDLR_MSB = 0x02F9;

/**
 * COM2 MSR (Modem Control Register) I/O address.
 */
const ioaddr iMCR = 0x02FC;

/**
 * COM2 IER (Interrupt Enable Register) I/O address.
 */
const ioaddr iIER = 0x02F9;

/**
 * COM2 IIR (Interrupt Identification Register) I/O address.
 */
const ioaddr iIIR = 0x02FA;

