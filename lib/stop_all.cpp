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
