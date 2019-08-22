/**
 * File: pci.h
 *       One of the major improvements the PCI Local Bus had over other I/O
 *       architectures was its configuration mechanism. In addition to the
 *       normal memory-mapped and I/O port spaces, each device function on the
 *       bus has a configuration space, which is 256 bytes long, addressable by
 *       knowing the eight-bit PCI bus, five-bit device, and three-bit function
 *       numbers for the device (commonly referred to as the BDF or B/D/F, as
 *       abbreviated from bus/device/function). This allows up to 256 buses,
 *       each with up to 32 devices, each supporting eight functions. A single
 *       PCI expansion card can respond as a device and must implement at least
 *       function number zero. The first 64 bytes of configuration space are
 *       standardized; the remainder are available for vendor-defined purposes.
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

