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

/*
 * Utility functions to work with the PCI bus.
 */

/**
 * Reads a byte from the register having internal offset regn relative to the
 * PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to read from
 * @param   dev     the device to read from
 * @param   fun     the PCI function to read from
 * @param   regn    the internal register to read from
 *
 * @return  the return byte from the register.
 */
natb pci_read_confb(natb bus, natb dev, natb fun, natb regn);

/**
 * Reads a word from the register having internal offset regn relative to the
 * PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to read from
 * @param   dev     the device to read from
 * @param   fun     the PCI function to read from
 * @param   regn    the internal register to read from
 *
 * @return  the return word from the register.
 */
natw pci_read_confw(natb bus, natb dev, natb fun, natb regn);

/**
 * Reads a long from the register having internal offset regn relative to the
 * PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to read from
 * @param   dev     the device to read from
 * @param   fun     the PCI function to read from
 * @param   regn    the internal register to read from
 *
 * @return  the return long from the register.
 */
natl pci_read_confl(natb bus, natb dev, natb fun, natb regn);

/**
 * Writes the byte data to the register having internal offset regn relative to
 * the PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to write to
 * @param   dev     the device to write to
 * @param   fun     the PCI function to write to
 * @param   regn    the internal register to write to
 * @param   data    the byte to be written to the register
 */
void pci_write_confb(natb bus, natb dev, natb fun, natb regn, natb data);

/**
 * Writes the word data to the register having internal offset regn relative to
 * the PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to write to
 * @param   dev     the device to write to
 * @param   fun     the PCI function to write to
 * @param   regn    the internal register to write to
 * @param   data    the word to be written to the register
 */
void pci_write_confw(natb bus, natb dev, natb fun, natb regn, natw data);

/**
 * Writes the long data to the register having internal offset regn relative to
 * the PCI function specified by fun of the device specified by dev.
 *
 * @param   bus     the bus to write to
 * @param   dev     the device to write to
 * @param   fun     the PCI function to write to
 * @param   regn    the internal register to write to
 * @param   data    the long to be written to the register
 */
void pci_write_confl(natb bus, natb dev, natb fun, natb regn, natl data);

/**
 * Looks for the PCI device specified using the deviceID and vendorID.
 * If found, returns true and the PCI device bus, device and function numbers
 * are passed by reference.
 *
 * @param   bus         the PCI device bus (if device found);
 * @param   dev         the PCI device number (if device found);
 * @param   fun         the PCI device function (if device found);
 * @param   vendorID    the PCI device vendor ID
 * @param   deviceID    the PCI device ID
 *
 * @return  true if the specified PCI device was found.
 */
bool pci_find_dev(natb& bus, natb& dev, natb& fun, natw vendorID, natw deviceID);

/**
 * Finds and returns (by reference) the class code for a given function of a
 * given device on a given bus.
 *
 * @param   bus     the PCI device bus;
 * @param   dev     the PCI device;
 * @param   fun     the PCI device function;
 * @param   code[]  the class code of the function;
 *
 * @return  true if the class code is found and false otherwise.
 */
bool pci_find_class(natb& bus, natb& dev, natb& fun, natb code[]);

/**
 * Finds the next PCI device starting from the given bus, device and function
 * number, if available.
 *
 * @param   bus   bus number;
 * @param   dev   device number;
 * @param   fun   function number;
 *
 * @return  true if a new PCI device is available.
 */
bool pci_next(natb& bus, natb& dev, natb& fun);

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

