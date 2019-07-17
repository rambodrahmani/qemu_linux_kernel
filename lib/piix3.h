/**
 * File: piix3.h
 *       PCI IDE ISA Xcelerator (PIIX), also known as Intel 82371, is a family
 *       of Intel southbridge microchips employed in some Intel chipsets.
 *       The PIIX integrated an IDE controller with two 8237 DMA controllers,
 *       the 8254 PIT, and two 8259 PICs and a PCI to ISA bus bridge. It was
 *       introduced with the 430FX Triton chipset in 1995.
 *       The PIIX3 introduced a USB 1.0 controller and support for an external
 *       I/O APIC. It was used with the 430HX and 430VX Triton II and 440FX
 *       northbridges.
 *
 *       The PCI-to-ISA bridge functions as a PCI target on the PCI bus. PCI
 *       transactions addressed to this target are forwarded to the ISA bus. If
 *       it is a read transfer, the core waits for all read data from the ISA
 *       slave and returns the data to the PCI bus. If it is a write transfer,
 *       the core posts the write data to its internal write buffer, terminates
 *       the PCI bus, and then writes the data to the ISA slave. With a typical
 *       PCI bus running at 33 MHz, this core operates the ISA bus at one-fourth
 *       the frequency of the PCI bus. The ISA bus operates at 8.33 MHz. The
 *       PCI-to-ISA bridge core supports 8-bit and 16-bit ISA bus devices, while
 *       the PCI bus is 32-bits wide. The core is capable of taking one PCI
 *       transfer and converting it into four or two transfers on the ISA bus.
 *       This megafunction sizes vary with features and customization. Eureka
 *       Technology can customize designs according to specific user
 *       requirements.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

/**
 *
 */
const natb PIIX3_APICBASE = 0x80;

/**
 *
 */
const natb PIIX3_XBCS = 0x4e;

/**
 *
 */
const natl PIIX3_XBCS_ENABLE = (1U << 8);

/**
 *
 */
const natw PIIX3_VENDOR_ID = 0x8086;

/**
 *
 */
const natw PIIX3_DEVICE_ID = 0x7000;

/**
 *
 */
bool piix3_ioapic_enable(natb bus, natb dev, natb fun);

/**
 *
 */
bool piix3_ioapic_disable(natb bus, natb dev, natb fun);

/*
 * PIIX
 * Alternatively referred to as Intel 82371FB (PIIX), Intel 82371SB (PIIX3), and
 * Intel 82371AB (PIIX4), PIIX is short for PCI IDE Xcelerator. It is an Intel
 * southbridge multi-function integrated circuit for PCI devices, and it
 * implements a PCI-to-ISA bridge and PCI IDE function. The PIIX3 implements a
 * Universal Serial Bus host/hub function.
 */

