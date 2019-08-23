/**
 * File: pci_find_dev.cpp
 *       Looks for the PCI device specified using the deviceID and vendorID.
 *       If found, returns true and the PCI device bus, device and function
 *       numbers are passed by reference.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "pci.h"

bool pci_find_dev(natb& bus, natb& dev, natb& fun, natw deviceID, natw vendorID)
{
    // loop through available PCI devices
    // (255 buses with 32 devices each with 8 functions)
    do {
        natw work;

        if ( (work = pci_read_confw(bus, dev, fun, 0)) == 0xFFFF )
        {
            continue;
        }

        // check vendor and device IDs
        if ( work == vendorID && pci_read_confw(bus, dev, fun, 2) == deviceID) 
        {
            // PCI device found
            return true;
        }
    } while (pci_next(bus, dev, fun));

    // PCI device not found
    return false;
}

/**
 * A PCI Bus has 3 addressable spaces: memory, I/O and configuration.
 * Each function of a device must implement, in the configuration space, certain
 * registers having size from 1 to 4 bytes. Some of this register are
 * compulsory:
 *  1. Vendor ID: (read-only, 2 bytes, offset 0) identifies the vendor;
 *  2. Device ID: (read-only, 2 bytes, offset 2) identifies the device;
 *  3. Command Register (read/write, 2 bytes, offset 4):
 *      - bit n. 0: if set, enables the function to accept transactions in the
 *                  I/O space;
 *      - bit n. 1: if set, enables the function to accept transactions in the
 *                  memory space;
 *      - bit n. 2: if set, enables the function to master the bus;
 *  4. Status Register: (read-only, 2 bytes, offset 6) contains information
 *                      regarding available functionalities and errors heppening
 *                      during transactions;
 *  5. Class Code: (read-only, 3 bytes, offset 9) defines the type of function
 *                 carried out according to a standard PCI (see the table
 *                 below);
 *  6. Interrupt Pin: (read-only, 1 byte, offset 57) used to define the pin used
 *                    to send interrupt requests.
 */

/**
 * Standard PCI Class Codes:
 *
 * Class Code   |         PCI Device
 *   (MSB)      |
 * ---------------------------------------------
 *    0x00      |       Pre revision 2.0
 *    0x01      |    Mass storage controller
 *    0x02      |      Network controller
 *    0x03      |      Display controller
 *    0x04      |       Multimedia device
 *    0x05      |       Memory controller
 *    0x06      |         Bridge device
 *    0x07      | Simple communication controller
 *    0x08      |      Base system peripheral
 *    0x09      |          Input device
 *    0x0A      |         Docking station
 *    0x0B      |           Processor
 *    0x0C      |       Serial bus controller
 *    0xFF      |             Other
 */

