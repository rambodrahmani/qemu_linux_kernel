/**
 *
 * File: internal.h
 *       This file contains utility methods to work with the Global Descriptor
 *       Table (GDT) and the Interrupt Descriptor Table (IDT).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019.
 *
 */

#include "libce.h"

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
 *
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
 *
 * @param   dest
 * @param   src
 * @param   l
 *
 * @return
 *
 */
char* strncpy(char* dest, const char* src, natl l);

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
 * @param   buf
 *
 * @return
 *
 */
extern "C" int puts(const char* buf);

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
 *
 */
extern "C" void cli();

/**
 *
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
 *
 */
void ini_COM1();

/**
 *
 */
void ini_COM2();

