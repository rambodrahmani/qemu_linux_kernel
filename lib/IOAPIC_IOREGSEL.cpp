/**
 * File: IOAPIC_IOREGSEL.cpp
 *       APIC IORegSel Register Address Definition.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "type.h"

/**
 * APIC IORegSel Register Address.
 */
natl* IOREGSEL = reinterpret_cast<natl*>(0xFEC0000);

