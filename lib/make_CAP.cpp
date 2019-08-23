/**
 * File: make_CAP.cpp
 *       The CPU is not capable of initiating bus transactions in the
 *       configuration space, and they must be generated indirectly using the
 *       guest-PCI bridge. To this end, the bridge has 2 32-bits long registers
 *       called CAP (Configuration Address Port) and CDP (Configuration Data
 *       Port), placed in the I/O space at addresses 0x0CF8 and 0x0CFC.
 *       In order to generate a transaction in the configuration space, the CPU
 *       must
 *          1. write a long word in the CAP register;
 *          2. read/write a byte, a word or a long word from/in the CDP
 *             register;
 *          3. the double word to be written in the CAP register must contains
 *             all the directives needed in order to address a long word in the
 *             configuration space for a given function of a given device.
 *       
 *       When a valid long word is written in the CAP register, the guest-PCI
 *       bridge will take care of executing a transaction in the configuration
 *       space which will effectively take place when a read/write operation is
 *       performed in the CDP register. When this happens, the guest-PCI bridge
 *       will save:
 *          1. the transaction type;
 *          2. the byte enable values.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"

natl make_CAP(natb bus, natb dev, natb fun, natb off)
{
	return 0x80000000 | (bus << 16) | (dev << 11) | (fun << 8) | (off & 0xFC);
}

/**
 * CAP Register
 * 31               23           16 15    11 10   8 7            0
 * ---------------------------------------------------------------
 * |               |      Bus      | Device | Func. |   Offset   |
 * ---------------------------------------------------------------
 */

