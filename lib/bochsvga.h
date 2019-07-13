/**
 * File: bochsvga.h
 *       All SVGA devices in qemu (except cirrus) have support for the bochs
 *       display interface. That interface was implemented by the bochs emulator
 *       first (this is where the name comes from). It was implemented in qemu
 *       too. For the qemu standard vga it is the primary interface. qxl-vga,
 *       virtio-vga and vmsvga support the bochs dispi interface when they are
 *       in vga compatibility mode, which is typically the case at boot, before
 *       the guest os loads the native display driver.
 *
 *       The bochs display interface is a paravirtual interface, with just the
 *       bare essentials to set video modes on a virtual display device. There
 *       are no registers for clock rate and other timing stuff for example.
 *
 *       Traditionally the bochs display interface uses I/O ports 0x1ce (bochs
 *       register index) and 0x1cf (bochs register data), As both registers are
 *       16bit the data registers is unaligned, which does not work on non-x86
 *       archs, so 0x1d0 is supported as data port too.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

struct bochsvga
{
	/**
     * 
     */
    void * framebuffer;

    /**
     *
     */
    natw * vgareg;

    /**
     *
     */
    natw * vbeext;

    /**
     * vbeext indexes.
     */

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_ID = 0x0;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_XRES = 0x1;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_YRES = 0x2;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_BPP = 0x3;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_ENABLE = 0x4;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_BANK = 0x5;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_VIRT_WIDTH = 0x6;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_VIRT_HEIGHT = 0x7;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_X_OFFSET = 0x8;

    /**
     *
     */
    static const natw VBE_DISPI_INDEX_Y_OFFSET = 0x9;

    //valori possibili per VBE_DISPI_INDEX_BPP

    /**
     *
     */
    static const natw VBE_DISPI_BPP_4 = 0x04;

    /**
     *
     */
    static const natw VBE_DISPI_BPP_8 = 0x08;

    /**
     *
     */
    static const natw VBE_DISPI_BPP_15 = 0x0F;

    /**
     *
     */
    static const natw VBE_DISPI_BPP_16 = 0x10;

    /**
     *
     */
    static const natw VBE_DISPI_BPP_24 = 0x18;

    /**
     *
     */
    static const natw VBE_DISPI_BPP_32 = 0x20;

    //bit settabili su VBE_DISPI_INDEX_ENABLE

    /**
     *
     */
    static const natw VBE_DISPI_DISABLED = 0x00;

    /**
     *
     */
    static const natw VBE_DISPI_ENABLED = 0x01;

    /**
     *
     */
    static const natw VBE_DISPI_GETCAPS = 0x02;

    /**
     *
     */
    static const natw VBE_DISPI_8BIT_DAC = 0x20;

    /**
     *
     */
    static const natw VBE_DISPI_LFB_ENABLED = 0x40;

    /**
     *
     */
    static const natw VBE_DISPI_NOCLEARMEM = 0x80;
};

/**
 * BOCHS VGA Video card instance.
 */
extern bochsvga videocard;

//inizializza il descrittore videocard (bochsvga)

/**
 * Initializes the BOCHS VGA Video card instance.
 */
bool bochsvga_init();

