/**
 * File: internal.h
 *       This file contains utility methods to work with the Global Descriptor
 *       Table (GDT) and the Interrupt Descriptor Table (IDT).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019.
 */

#include "libqlk.h"

/**
 *
 */
extern "C" void init_gdt();

/**
 *
 */
extern "C" void load_gdt();

/**
 *
 */
extern "C" void unload_gdt();

/**
 *
 * @param   num
 * @param   base
 * @param   limit
 * @param   pres
 * @param   dpl
 * @param   type
 * @param   gran
 *
 */
extern "C" void init_descrittore(natl num, addr base, natl limit, natl pres, natl dpl, natl type, natl gran);

/**
 *
 */
extern "C" void init_idt();

/**
 *
 */
extern "C" void idt_reset();

/**
 *
 */
extern "C" void delay_init();

/**
 * When the boot-loader is done switching from Real Mode to Protected Mode, and
 * the user program is loaded into memory, the execution starts from the start
 * entry point. The init_all function is automatically called which will cause
 * the subsequent calls to the following library functions: init_gdt(),
 * load_gdt(), init_idt(), bochsvga_init(), clear_screen(), init_COM1(),
 * init_COM2(), apic_init(), apic_reset(), keyboard_init(), delay_init(), ini(),
 * sti() and finally main(), which contains the user program instructions.
 */
extern "C" bool init_all();

/**
 *
 */
extern "C" void stop_all();

/**
 *
 * @param   buf
 * @param   len
 * @param   l
 */
void itostr(char* buf, unsigned int len, long l);

/**
 *
 * @param   buf
 * @param   l
 * @param   digits
 *
 */
void htostr(char* buf, unsigned long long l, int digits = 8);

/**
 *
 * @param   l
 * @param   out
 * @param   digits
 *
 * @return
 *
 */
char* convnat(natl l, char* out, int digits);

/**
 * Copies the first num characters of source to destination. If the end of the
 * source C string (which is signaled by a null-character) is found before num
 * characters have been copied, destination is padded with zeros until a total
 * of num characters have been written to it.
 *
 * @param   source       source string to be copied;
 * @param   destination  destination string for the copy;
 * @param   num          number of chars to be copied;
 *
 * @return  destination is returned.
 */
char* strncpy(const char* source, char* destination, natl num);

/**
 *
 * @param   buf
 * @param   n
 * @param   fmt
 *
 * @return
 *
 */
int snprintf(char* buf, natl n, const char* fmt, ...);

/**
 *
 * @param   fmt
 *
 * @return
 *
 */
int printf(const char* fmt, ...);

/**
 *
 * @param   code
 *
 * @return
 *
 */
const char* pci_decode_class(natb code);

/**
 *
 */
extern "C" void apic_send_EOI();

/**
 * Clears interrupt flag; interrupts disabled when interrupt flag cleared. See
 * 64/cli.s for details.
 */
extern "C" void cli();

/**
 * Sets the interrupt flag; external, maskable interrupts enabled at the end of
 * the next instruction. See 64/sti.s for details.
 */
extern "C" void sti();

/**
 *
 * @param   msg
 */
extern "C" void panic(const char* msg);

#ifdef X86_64
    #define line natq
#else
    #define line natl
#endif

/**
 * Initializes serial interface COM1.
 */
void init_COM1();

/**
 * Initializes serial interface COM2.
 */
void init_COM2();

