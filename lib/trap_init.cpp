/**
 * File: trap_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"

/**
 * IDT Gate Structure.
 *
 * 32                                   0
 * --------------------------------------
 * |              ADDRESS               |
 * --------------------------------------
 * |P|   ...   |TI|                     |
 * --------------------------------------
 *  ^ACCESS BYTE^
 */
struct gate
{
    // address byte
    natl dw1;

    // access byte
    natl dw2;

    #ifdef X86_64
        natl dw3;
        natl dw4;
    #endif
};

/**
 * IDT extern pointer.
 *
 * [/64/idt.s]
 */
extern gate idt[];

/**
 * Creates an IDT gate at the given index for the given subroutine.
 *
 * [/64/create_gate.s]
 */
extern "C" void create_gate(gate& g, void routine(), bool trap);

void trap_init(natl num, void routine())
{
    gate gg;

    create_gate(gg, routine, true /* trap */);

    idt[num] = gg;
}

