/**
 * File: bochsvga_config.cpp
 *       Sets the video card graphic mode and resolution.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "bochsvga.h"

natb* bochsvga_config(natw max_screenx, natw max_screeny)
{
    // check if the video card has been initialized
	if (videocard.framebuffer == 0)
    {
        // return if not
        return 0;
    }

    // disable BOCHS VBE extensions, as required
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_ENABLE] =
        bochsvga::VBE_DISPI_DISABLED;

    // set color resolution and depth
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_XRES] = max_screenx;
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_YRES] = max_screeny;
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_BPP] = bochsvga::VBE_DISPI_BPP_8;

    // enable BOCHS VBE extensions and linear buffer mode
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_ENABLE] =
        bochsvga::VBE_DISPI_ENABLED | bochsvga::VBE_DISPI_LFB_ENABLED;

    return static_cast<natb*>(videocard.framebuffer);
}

