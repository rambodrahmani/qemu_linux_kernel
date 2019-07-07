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
    cli();
    keyboard_reset();
    clear_screen(0x07);
    apic_reset();
    idt_reset();
    unload_gdt();
}
