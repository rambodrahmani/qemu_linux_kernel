/**
 *
 * File: lib.h
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019
 *
 */

#include "type.h"

/*
 * Utility functions to read and write to a registry of the I/O space.
 */

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputb(ioaddr reg, natb &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputb(natb a, ioaddr reg);

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputw(ioaddr reg, natw &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputw(natw a, ioaddr reg);

/**
 *
 * @param   reg
 * @param   &a
 *
 */
extern "C" void inputl(ioaddr reg, natl &a);

/**
 *
 * @param   a
 * @param   reg
 *
 */
extern "C" void outputl(natl a, ioaddr reg);

/**
 *
 * @param   vect[]
 * @param   length
 * @param   reg
 *
 */
extern "C" void outputbw(natw vect[], int length, ioaddr reg);

/**
 *
 * @param   reg
 * @param   vect[]
 * @param   length
 *
 */
extern "C" void inputbw(ioaddr reg, natw vect[], int length);

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

