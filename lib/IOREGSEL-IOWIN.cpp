/**
 * File: IOREGSEL-IOWIN.cpp
 *       APIC accessible registers.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "type.h"

/**
 * APIC IORegSel Register Address.
 */
natl* IOREGSEL = reinterpret_cast<natl*>(0xFEC0000);

/**
 * APIC IOWin Register Address.
 */
natl* IOWIN = reinterpret_cast<natl*>(0xFEC0010);

