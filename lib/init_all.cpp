/**
 * File: init_all.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"
#include "kbd.h"
#include "bochsvga.h"

extern void ini();

bool init_all()
{
    init_gdt();

    load_gdt();
    
    init_idt(); // chiama gate_init per le prime 32 entrate
    
    bochsvga_init();
    
    clear_screen(0x4b);
    
    ini_COM1();
    
    ini_COM2();
    
    apic_init();
    
    apic_reset();
    
    keyboard_init();
    
    delay_init();
    
    ini();
    
    sti();

    return true;
}
