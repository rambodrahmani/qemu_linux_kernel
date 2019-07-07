/**
 * File: init_all.cpp
 *       When the boot-loader is done switching from Real Mode to Protected
 *       Mode, and the user program is loaded into memory, the execution starts
 *       from the start entry point. The init_all function is automatically
 *       called which will cause the subsequent calls to the following library
 *       functions: init_gdt(), load_gdt(), init_idt(), bochsvga_init(),
 *       clear_screen(), ini_COM1(), ini_COM2(), apic_init(), apic_reset(),
 *       keyboard_init(), delay_init(), ini(), sti() and finally main(), which
 *       contains the user program instructions.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/07/2019.
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

