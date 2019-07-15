/**
 * File: hdd.h
 *       This file contains constans related to the disk controller interface.
 *
 *       The disk controller is the controller circuit which enables the CPU to
 *       communicate with a hard disk, floppy disk or other kind of disk drive.
 *       Also it provides an interface between the disk drive and the bus
 *       connecting it to the rest of the system.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 * BR (Buffer Register) I/O address.
 */
const ioaddr iBR = 0x01F0;

/**
 * CNL (Cylinder Number Low) Register I/O address.
 */
const ioaddr iCNL = 0x01F4;

/**
 * CNH (Cylinder Number High) Register I/O address.
 */
const ioaddr iCNH = 0x01F5;

/**
 * SNR (Sector Number Register) I/O address.
 */
const ioaddr iSNR = 0x01F3;

/**
 * HND (Head Number and Drive) Register I/O address.
 */
const ioaddr iHND = 0x01F6;

/**
 * SCR (Sector Count Register) I/O address.
 */
const ioaddr iSCR = 0x01F2;

/**
 * ERR (Error Register) I/O address.
 */
const ioaddr iERR = 0x01F1;

/**
 * CMD (Command) Register I/O address.
 */
const ioaddr iCMD = 0x01F7;

/**
 * STS (Status) Register I/O address.
 */
const ioaddr iSTS = 0x01F7;

/**
 * DCR (Digital Control Register) I/O address.
 */
const ioaddr iDCR = 0x03F6;

