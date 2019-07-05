/**
 * File: liblk.h
 *       This file contains utility methods declarations.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019.
 */

#include "type.h"
#include <stdarg.h>

/*
 * Utility functions to read and write to a registry of the I/O space.
 */

/**
 * Transfers a byte from the I/O port addressed by reg to b.
 *
 * @param   reg  source I/O port address
 * @param   &b   destination byte
 */
extern "C" void inputb(ioaddr reg, natb &b);

/**
 * Transfers the byte contained in b to the I/O port addressed by reg.
 *
 * @param   b    source byte
 * @param   reg  destination I/O port address
 */
extern "C" void outputb(natb b, ioaddr reg);

/**
 * Transfers a word from the I/O port addressed by reg to w.
 *
 * @param   reg  source I/O port address
 * @param   &w   destination word
 *
 */
extern "C" void inputw(ioaddr reg, natw &w);

/**
 * Transfers the word contained in w to the I/O port addressed by reg.
 *
 * @param   w    source word
 * @param   reg  destination I/O port address
 */
extern "C" void outputw(natw w, ioaddr reg);

/**
 * Transfers a long from the I/O port addressed by reg to l.
 *
 * @param   reg  source I/O port address
 * @param   &l   destination long
 */
extern "C" void inputl(ioaddr reg, natl &l);

/**
 * Transfers the long contained in l to the I/O port addressed by reg.
 *
 * @param   l    source long
 * @param   reg  destination I/O port address
 */
extern "C" void outputl(natl l, ioaddr reg);

/**
 *
 * @param   reg
 * @param   vect[]
 * @param   length
 */
extern "C" void inputsw(ioaddr reg, natw vect[], int length);

/**
 *
 * @param   vect[]
 * @param   length
 * @param   reg
 */
extern "C" void outputsw(natw vect[], int length, ioaddr reg);

/*
 * Utility functions to read from the keyboard.
 */

/**
 * @return 
 */
natb get_code();

/**
 * @return 
 */
char char_read();

/**
 *
 * @param   c
 *
 * @return
 */
char conv(natb c);

/*
 * Utility functions to setup the video ourput.
 */

/**
 *
 * @param   screen_max_x
 * @param   screen_max_y
 *
 */
natb* bochsvga_config(natw screen_max_x, natw screen_max_y);

/*
 * Utility functions to write to the video output.
 */

/**
 *
 * @param   col
 *
 */
void clear_screen(natb col);

/**
 *
 * @param c
 *
 */
void char_write(natb c);

/**
 *
 * @param str[]
 *
 */
void str_write(const char str[]);

/*
 * Utility functions to read and write to the primary hard disk drive.
 */

/**
 *
 * @param   lba
 * @param   length
 * @param   vect[]
 *
 */
void hdd_read(natl lba, natb length, natw vect[]);

/**
 *
 * @param   lba
 * @param   length
 * @param   vect[]
 *
 */
void hdd_write(natl lba, natb length, natw vect[]);

/*
 * Utility functions for the serial port.
 */

/**
 *
 * @param c
 *
 */
void serial_o(natb c);

/**
 *
 * @param c
 *
 */
void serial2_o(natb c);

/*
 * Utility functions to work with strings.
 */

/**
 *
 * @param   dest
 * @param   c
 * @param   n
 *
 */
void *memset(void* dest, int c, size_t n);

/**
 *
 * @param   dest
 * @param   src
 * @param   n
 *
 */
void *memcpy(void* dest, const void* src, size_t n);

/**
 *
 * @param str[]
 *
 * @return
 *
 */
size_t strlen(const char str[]);

/**
 *
 * @param   src[]
 * @param   dst[]
 *
 * @return
 *
 */
char* copy(const char src[], char dst[]);

/**
 *
 * @param   n
 * @param   dst[]
 *
 * @return
 *
 */
char* natb_conv(natb n, char dst[]);

/**
 *
 * @param   n
 * @param   dst[]
 *
 * @return
 *
 */
char* natw_conv(natw n, char dst[]);

/**
 *
 * @param   n
 * @param   dst[]
 *
 * @return
 *
 */
char* natl_conv(natl n, char dst[]);

/**
 *
 * @param   n
 * @param   dst[]
 *
 * @return
 *
 */
char* natq_conv(natq n, char dst[]);

/**
 *
 * @param   n
 * @param   dst[]
 *
 * @return
 *
 */
char* int_conv(long n, char dst[]);

/*
 * Utility functions to wait.
 */

/**
 *
 */
void pause();

/*
 * Utility functions to work with the interrupt mechanism.
 */

/**
 *
 * @param   num
 * @param   routine()
 *
 */
extern "C" void gate_init(natl num, void routine());

/**
 *
 * @param   num
 * @param   routine()
 *
 */
extern "C" void trap_init(natl num, void routine());

/**
 *
 * @param   irq
 * @param   enable
 *
 */
extern "C" void apic_set_MIRQ(natl irq, bool enable);

/**
 *
 * @param   irq
 * @param   vec
 *
 */
extern "C" void apic_set_VECT(natl irq, natb vec);

/*
 * Utility functions for thread synchronization.
 */

/**
 *
 * @param   sem
 *
 */
extern "C" void sem_wait(natl sem);

/**
 *
 * @param   sem
 *
 */
extern "C" void sem_signal(natl sem);

/*
 * Utility functions to implment time delay.
 */

/**
 *
 * @param   num
 *
 */
extern "C" void delay(natl num);

/*
 * Utility functions to work with the PCI bus.
 */

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 *
 * @return
 *
 */
natb pci_read_confb(natb bus, natb dev, natb fun, natb regn);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 *
 * @return
 *
 */
natw pci_read_confw(natb bus, natb dev, natb fun, natb regn);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 *
 * @return
 *
 */
natl pci_read_confl(natb bus, natb dev, natb fun, natb regn);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 * @param   data
 *
 */
void pci_write_confb(natb bus, natb dev, natb fun, natb regn, natb data);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 * @param   data
 *
 */
void pci_write_confw(natb bus, natb dev, natb fun, natb regn, natw data);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   regn
 * @param   data
 *
 */
void pci_write_confl(natb bus, natb dev, natb fun, natb regn, natl data);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   vendorID
 * @param   deviceID
 *
 * @return
 *
 */
bool pci_find_dev(natb& bus, natb& dev, natb& fun, natw vendorID, natw deviceID);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 * @param   code[]
 *
 * @return
 *
 */
bool pci_find_class(natb& bus, natb& dev, natb& fun, natb code[]);

/**
 *
 * @param   bus
 * @param   dev
 * @param   fun
 *
 * @return
 *
 */
bool pci_next(natb& bus, natb& dev, natb& fun);

/*
 * Utility functions to work with Heap Memory.
 */

/**
 *
 * @param   v
 * @param   a
 *
 * @return
 *
 */
size_t allinea(size_t v, size_t a);

/**
 *
 * @param   start
 * @param   size
 *
 */
void heap_init(void* start, size_t size);

/**
 *
 * @param   dim
 */
void* alloca(size_t dim);

/**
 *
 * @param   p
 */
void dealloca(void* p);

/**
 *
 * @return
 *
 */
size_t disponibile();

/*
 * Utility functions for logging.
 */

/**
 * This enumeration contains the severity which can be used for a log.
 */
enum log_sev
{
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERR,
    LOG_USR
};

/**
 *
 * @param   sev
 * @param   fmt
 *
 */
extern "C" void flog(log_sev sev, const char* fmt, ...);

/**
 *
 * @param   sev
 * @param   buf
 * @param   quanti
 *
 */
extern "C" void do_log(log_sev sev, const char* buf, natl quanti);

/*
 * Advanced utility functions.
 */

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
 * @param   str
 * @param   size
 * @param   fmt
 * @param   ap
 *
 * @return 
 */
int vsnprintf(char* str, size_t size, const char* fmt, va_list ap);

/**
 *
 * @param   buf
 * @param   n
 * @param   fmt
 *
 * @return
 */
int snprintf(char* buf, natl n, const char* fmt, ...);

/**
 *
 * @return
 *
 */
bool apic_init();

/**
 *
 * @return
 *
 */
void apic_reset();

