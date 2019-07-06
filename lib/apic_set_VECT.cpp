/**
 * File: apic_set_VECT.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_set_VECT(natl ir, natb vec)
// ir: numero d'ordine del piedino
// vec: tipo che deve essere associato
{
    natl work;
	// lettura della prima parola lunga dell'entrata
	// ir-esima della tabella interna
	*IOREGSEL = 16 + ir * 2;
	work = *IOWIN;
	// azzeramento (&) e predisposizione (|) dei bit 7-0 con vec
	work &= 0xFFFFFF00;
	work |= vec;
	// scrittura nella prima parola lunga dell'entrata
	// ir-esima della tabella interna
	*IOREGSEL = 16 + ir * 2;
	*IOWIN = work;
}
