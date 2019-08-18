/**
 * File: pci.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

/**
 * PCI Bus Configuration Address Port (CAP) register. [0]
 */
const ioaddr PCI_CAP = 0x0CF8;

/**
 * PCI Bus Configuration Data Port (CDP) register.
 */
const ioaddr PCI_CDP = 0x0CFC;

/**
 * Generates the long word to be written in the CAP register by the processor
 * in order to start.
 *
 * @param   bus  
 * @param   dev  
 * @param   fun  
 * @param   off  
 *
 * @return  the  long word to be written in the CAP register.
 */
natl make_CAP(natb bus, natb dev, natb fun, natb off);

/**
 * [0]
 * CAP Register bits:
 *  n. 31:    set to 1 to identify a configuration transaction;
 *  n. 30-24: reserved;
 *  n. 23-16: number of the bus where the device is located;
 *  n. 15-11: number of the device within the bus;
 *  n. 10-8:  number of the function within the device;
 *  n. 7-2:   offset of the long world involved in the configuration
 *            transaction;
 *  n. 1-0:   set to 0.
 */

