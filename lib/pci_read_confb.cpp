/**
 * File: pci_read_confb.cpp
 *       Reads a byte from the CDP register having internal offset regn relative
 *       to the PCI function specified by fun of the device specified by dev.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

natb pci_read_confb(natb bus, natb dev, natb fun, natb off)
{
    natb ret;

    natl confaddr = make_CAP(bus, dev, fun, off);
    outputl(confaddr, PCI_CAP);
    inputb(PCI_CDP + (off & 0x03), ret);

    return ret;
}

/**
 * PCI Configuration Headers
 * Every PCI device in the system, including the PCI-PCI bridges has a
 * configuration data structure that is somewhere in the PCI configuration
 * address space. The PCI Configuration header allows the system to identify
 * and control the device. Exactly where the header is in the PCI Configuration
 * address space depends on where in the PCI topology that device is. For
 * example, a PCI video card plugged into one PCI slot on the PC motherboard
 * will have its configuration header at one location and if it is plugged into
 * another PCI slot then its header will appear in another location in PCI
 * Configuration memory. This does not matter, for wherever the PCI devices and
 * bridges are the system will find and configure them using the status and
 * configuration registers in their configuration headers. Typically systems are
 * designed so that every PCI slot has it's PCI Configuration Header in an
 * offset that is related to its slot on the board. So, for example, the first
 * slot on the board might have its PCI Configuration at offset 0 and the second
 * slot at offset 256 (all headers are the same length, 256 bytes) and so on. A
 * system specific hardware mechanism is defined so that the PCI configuration
 * can attempt to examine all possible PCI Configuration Headers for a given PCI
 * bus and know which devices are present and which devices are absent simply by
 * trying to read one of the fields in the header (usually the Vendor
 * Identification field) and getting some sort of error. The [3, PCI Local Bus
 * Specification,] describes one possible error message as returning 0xFFFFFFFF
 * when attempting to read the Vendor Identification and Device Identification
 * fields for an empty PCI slot.
 *
 * Vendor Identification
 *  A unique number describing the originator of the PCI device. Digital's PCI
 *  Vendor Identification is 0x1011 and Intel's is 0x8086.
 *
 * Device Identification
 *  A unique number describing the device itself. For example, Digital's 21141
 *  fast ethernet device has a device identification of 0x0009.
 *
 * Status
 *  This field gives the status of the device with the meaning of the bits of
 *  this field set by the standard. [3, PCI Local Bus Specification,].
 *
 * Command
 *  By writing to this field the system controls the device, for example
 *  allowing the device to access PCI I/O memory,
 *
 * Class Code
 *  This identifies the type of device that this is. There are standard classes
 *  for every sort of device; video, scsi and so on. The class code for SCSI is
 *  0x0100.
 *
 * Base Address Registers
 *  These registers are used to determine and allocate the type, amount and
 *  location of PCI I/O and PCI memory space that the device can use.
 *
 * Interrupt Pin
 *  Four of the physical pins on the PCI card carry interrupts from the card to
 *  the PCI bus. The standard labels these as A, B, C and D. The Interrupt Pin
 *  field describes which of these pins this PCI device uses. Generally it is
 *  hardwired for a pariticular device. That is, every time the system boots the
 *  device uses the same interrupt pin. This information allows the interrupt
 *  handling subsystem to manage interrupts from this device,
 *
 * Interrupt Line
 *  The Interrupt Line field of the device's PCI Configuration header is used to
 *  pass an interrupt handle between the PCI initialisation code, the device's
 *  driver and Linux's interrupt handling subsystem. The number written there is
 *  meaningless to the the device driver but it allows the interrupt handler to
 *  correctly route an interrupt from the PCI device to the correct device
 *  driver's interrupt handling code within the Linux operating system.
 */

