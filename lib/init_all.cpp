/**
 * File: init_all.cpp
 *       When the boot-loader is done switching from Real Mode to Protected
 *       Mode, and the user program is loaded into memory, the execution starts
 *       from the start entry point. The init_all function is automatically
 *       called which will cause the subsequent calls to the following library
 *       functions: init_gdt(), load_gdt(), init_idt(), bochsvga_init(),
 *       clear_screen(), init_COM1(), init_COM2(), apic_init(), apic_reset(),
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
    // initialize the GDT
    init_gdt();

    load_gdt();
    
    // init the IDT (first 32 entries only)
    init_idt();
    
    bochsvga_init();
    
    clear_screen(0x4b);
    
    // initialize serial interface COM1
    init_COM1();
    
    // initialize serial interface COM2
    init_COM2();
    
    apic_init();
    
    apic_reset();
    
    // initialize keyboard
    keyboard_init();
    
    delay_init();
    
    ini();
    
    sti();

    return true;
}

