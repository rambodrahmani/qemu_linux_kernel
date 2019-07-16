/**
 * File: trap_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

/**
 * IDT Gate structure.
 */
struct gate
{
	natl dw1;
	natl dw2;

    #ifdef X86_64
	    natl dw3;
	    natl dw4;
    #endif
};

extern gate idt[];

extern "C" void create_gate(gate& g, void routine(), bool trap);

void trap_init(natl num, void routine())
{
	gate gg;

	create_gate(gg, routine, true /* trap */);

	idt[num] = gg;
}

