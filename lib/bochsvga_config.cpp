/**
 * File: bochsvga_config.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "bochsvga.h"

// imposta la modalità grafica sulla scheda video, scegliendone la risoluzione
natb* bochsvga_config(natw max_screenx, natw max_screeny)
{
	if(videocard.framebuffer == 0)
    {
        return 0;
    }

    // innanzitutto disablitiamo le estensioni BOCHS VBE, come richiesto da specifiche
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_ENABLE] = bochsvga::VBE_DISPI_DISABLED;

    // settiamo risoluzione e profondità di colore
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_XRES] = max_screenx;
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_YRES] = max_screeny;
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_BPP] = bochsvga::VBE_DISPI_BPP_8;

    // chiediamo la modalità di buffer lineare e riattiviamo le estensioni
    videocard.vbeext[bochsvga::VBE_DISPI_INDEX_ENABLE] = bochsvga::VBE_DISPI_ENABLED | bochsvga::VBE_DISPI_LFB_ENABLED;

    return static_cast<natb*>(videocard.framebuffer);
}

