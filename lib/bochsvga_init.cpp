/**
 * File: bochsvga_init.cpp
 *       Initializes the BOCHS VGA Video card instance.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "pci.h"
#include "bochsvga.h"

bool bochsvga_init()
{
    natb bus = 0, dev = 0, fun = 0;

    // video card device and vendor id
    natw deviceID = 0x1234;
    natw vendorID = 0x1111;

    // find the video card using the given device and vendor id
    if(!pci_find_dev(bus, dev, fun, vendorID, deviceID))
    {
        // video card not found
        flog(
            LOG_WARN,
            "bochsvga: video card not found"
        );

        return false;
    }

    // set framebuffer memory address
    videocard.framebuffer = reinterpret_cast<void*>(
            pci_read_confl(bus, dev, fun, 0x10) & ~0xf
    );

    // set vga controller register memory address
    videocard.vgareg = reinterpret_cast<natw*>(
            (pci_read_confl(bus, dev, fun, 0x18) & ~0xf) + 0x0400
    );

    // set BOCHS VBE extensions registers memory address
    videocard.vbeext = reinterpret_cast<natw*>(
            (pci_read_confl(bus, dev, fun, 0x18) & ~0xf) + 0x0500
    );

    // log message
    flog(
        LOG_INFO,
        "bochsvga: framebuffer initialized on %p and vbext on %p",
        videocard.framebuffer,
        videocard.vbeext
    );

    return true;
}

