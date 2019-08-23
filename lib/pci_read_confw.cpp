/**
 * File: pci_read_confw.cpp
 *       Reads a word from the CDP register having internal offset regn relative
 *       to the PCI function specified by fun of the device specified by dev.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

natw pci_read_confw(natb bus, natb dev, natb fun, natb off)
{
    natw ret;

    natl confaddr = make_CAP(bus, dev, fun, off);
    outputl(confaddr, PCI_CAP);
    inputw(PCI_CDP + (off & 0x03), ret);

    return ret;
}

/**
 * PCI Address Spaces 
 * The CPU and the PCI devices need to access memory that is shared between
 * them. This memory is used by device drivers to control the PCI devices and to
 * pass information between them. Typically the shared memory contains control
 * and status registers for the device. These registers are used to control the
 * device and to read its status. For example, the PCI SCSI device driver would
 * read its status register to find out if the SCSI device was ready to write a
 * block of information to the SCSI disk. Or it might write to the control
 * register to start the device running after it has been turned on. The CPU's
 * system memory could be used for this shared memory but if it were then every
 * time a PCI device accessed memory, the CPU would have to stall waiting for
 * the PCI device to finish. Access to memory is generally limited to one system
 * component at a time. This would slow the system down. It is also not a good
 * idea to allow the system's peripheral devices to access main memory in an
 * uncontrolled way. This would be very dangerous; a rogue device could make the
 * system very unstable.
 *
 * Peripheral devices have their own memory spaces which they use. The CPU can
 * access these spaces but access by the devices into the system's memory is
 * very strictly controlled using DMA (Direct Memory Access) channels. ISA
 * devices have access to two address spaces, ISA I/O (Input/Output) and ISA
 * memory. PCI has three; PCI I/O, PCI Memory and PCI Configuration space. All
 * of these address spaces are also accessible by the CPU with the the PCI I/O
 * and PCI Memory address spaces being used by the device drivers and the PCI
 * Configuration space being used by the PCI initialization code within the
 * Linux kernel.
 */

