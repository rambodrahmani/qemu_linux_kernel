/**
 * File: stop_all.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
 */

#include "internal.h"
#include "apic.h"
#include "kbd.h"

void stop_all()
{
    // clear interrupt flags
    cli();

    // reset the keyboard: interrupt requests 
    keyboard_reset();

    // clear video output background color
    clear_screen(0x07);

    // reset the APIC pins
    apic_reset();

    //
    idt_reset();

    //
    unload_gdt();
}

