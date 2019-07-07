/**
 * File: pci.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

/**
 *
 */
const ioaddr PCI_CAP = 0x0CF8;

/**
 *
 */
const ioaddr PCI_CDP = 0x0CFC;

/**
 *
 */
natl make_CAP(natb bus, natb dev, natb fun, natb off);
