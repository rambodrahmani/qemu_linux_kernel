/**
 * File: piix3.h
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

