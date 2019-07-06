/**
 * File: apic.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 *
 */
const natl IOAPIC_DEST_MSK = 0xFF000000; // destination field mask

/**
 *
 */
const natl IOAPIC_DEST_SHF = 24;         // destination field shift

/**
 *
 */
const natl IOAPIC_MIRQ_BIT = (1U << 16); // mask irq bit

/**
 *
 */
const natl IOAPIC_TRGM_BIT = (1U << 15); // trigger mode (1=level, 0=edge)

/**
 *
 */
const natl IOAPIC_IPOL_BIT = (1U << 13); // interrupt polarity (0=high, 1=low)

/**
 *
 */
const natl IOAPIC_DSTM_BIT = (1U << 11); // destination mode (0=physical, 1=logical)

/**
 *
 */
const natl IOAPIC_DELM_MSK = 0x00000700; // delivery mode field mask (000=fixed)

/**
 *
 */
const natl IOAPIC_DELM_SHF = 8;          // delivery mode field shift

/**
 *
 */
const natl IOAPIC_VECT_MSK = 0x000000FF; // vector field mask

/**
 *
 */
const natl IOAPIC_VECT_SHF = 0;          // vector field shift

/**
 *
 */
const natl IOAPIC_RTO = 0x10;

/**
 *
 */
const natl IRQ_MAX = 24;

/**
 *
 */
const natl IRQ_VECT0 = 0x20;

/**
 *
 */
extern natl* IOAPIC_EOIR;

/**
 *
 */
extern natl* IOREGSEL;

/**
 *
 */
extern natl* IOWIN;

/**
 *
 */
void apic_dump();

/**
 *
 */
void apic_dump_rt(natb start, natb end);

/**
 *
 */
natl apic_in(natb off);

/**
 *
 */
bool apic_init();

/**
 *
 */
void apic_reset();

/**
 *
 */
void apic_out(natb off, natl v);

/**
 *
 */
natl apic_read_rth(natb irq);

/**
 *
 */
natl apic_read_rtl(natb irq);

/**
 *
 */
extern "C" void apic_send_EOI();

/**
 *
 */
void apic_set_DEST(natl irq, natb dest);

/**
 *
 */
void apic_set_IPOL(natl irq, bool v);

/**
 *
 */
void apic_set_MIRQ(natl irq, bool v);

/**
 *
 */
void apic_set_TRGM(natl irq, bool v);

/**
 *
 */
void apic_set_VECT(natl irq, natb vec);

/**
 *
 */
void apic_write_rth(natb irq, natl w);

/**
 *
 */
void apic_write_rtl(natb irq, natl w);
