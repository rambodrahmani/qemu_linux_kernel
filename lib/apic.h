/**
 * File: apic.h
 *       In computing, Intel's Advanced Programmable Interrupt Controller (APIC)
 *       is a family of interrupt controllers. As its name suggests, the APIC is
 *       more advanced than Intel's 8259 Programmable Interrupt Controller
 *       (PIC), particularly enabling the construction of multiprocessor
 *       systems. It is one of several architectural designs intended to solve
 *       interrupt routing efficiency issues in multiprocessor computer systems.
 *
 *       In the following definitions, keep in mind that
 *          1U = 00000000000000000000000000000001.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

/**
 * Destination Field Mask.
 */
const natl IOAPIC_DEST_MSK = 0xFF000000;

/**
 * Destination Field Shift.
 */
const natl IOAPIC_DEST_SHF = 24;

/**
 * Interrupt mask. Stops the interrupt from reaching the processor if set.
 */
const natl IOAPIC_MIRQ_BIT = (1U << 16); // mask irq bit

/**
 * Trigger mode. 0 = Edge sensitive, 1 = Level sensitive.
 */
const natl IOAPIC_TRGM_BIT = (1U << 15);

/**
 * Polarity of the interrupt. 0 = High is active, 1 = Low is active.
 */
const natl IOAPIC_IPOL_BIT = (1U << 13);

/**
 * Destination mode. Affects how the destination field is read, 0 is physical
 * mode, 1 is logical. If the Destination Mode of this entry is Physical Mode,
 * bits 56-59 contain an APIC ID.
 */
const natl IOAPIC_DSTM_BIT = (1U << 11);

/**
 * Type of delivery mode. 0 = Normal, 1 = Low priority, 2 = System management
 * interrupt, 4 = Non maskable interrupt, 5 = INIT, 7 = External.
 * All others are reserved.
 */
const natl IOAPIC_DELM_MSK = 0x00000700;

/**
 * Delivery Mode Field Shift.
 */
const natl IOAPIC_DELM_SHF = 8;

/**
 * Vector Field Mask.
 * Interrupt vector. Allowed values are from 0x10 to 0xFE. 
 */
const natl IOAPIC_VECT_MSK = 0x000000FF;

/**
 * Vector Field Shift.
 */
const natl IOAPIC_VECT_SHF = 0;

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
 * EOI Register Address. See IOAPIC_EOIR.cpp.
 */
extern natl* IOAPIC_EOIR;

/**
 * IORegSel Register Address. See IOAPIC_IOREGSEL.cpp.
 */
extern natl* IOREGSEL;

/**
 * IOWin Register Address. See IOAPIC_IOWIN.cpp.
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
 * Send the End Of Interrupt (EOI). An End Of Interrupt (EOI) is a signal sent
 * to a Programmable Interrupt Controller (PIC) to indicate the completion of
 * interrupt processing for a given interrupt.
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
 * Enables (disable = false) or disables (disable = true) the given APIC
 * ir pin.
 *
 * @param   ir      ir pin index number
 * @param   disable false to enable and true to disable the ir pin
 */
extern "C" void apic_set_MIRQ(natl ir, bool disable);

/**
 * Sets the given type for the interrupts coming from the specified APIC ir pin.
 *
 * @param   ir    ir pin index number
 * @param   type  interrupt type
 */
extern "C" void apic_set_VECT(natl ir, natb type);

/**
 * Sets the trigger mode for the specified APIC ir pin.
 *
 * @param   ir      ir pin index number
 * @param   mode    true  IO-APIC-level
 *                  false IO-APIC-edge
 */
void apic_set_TRGM(natl ir, bool mode);

/**
 *
 */
void apic_write_rth(natb irq, natl w);

/**
 *
 */
void apic_write_rtl(natb irq, natl w);

