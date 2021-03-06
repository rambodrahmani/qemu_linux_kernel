/**
 * File: libqlk.h
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
 * Transfers a sequence of length words from the I/O port addressed by reg to
 * the vector vectw[].
 *
 * @param   reg      source I/O port address
 * @param   vectw[]  vector of words to be transferred
 * @param   length   number of words to be transferred
 */
extern "C" void inputsw(ioaddr reg, natw vectw[], int length);

/**
 * Transfers a sequence of length words contained in vectw to the I/O port
 * addressed by reg.
 *
 * @param   vectw[]  vector of words to be transferred
 * @param   length   number of words to be transferred
 * @param   reg      destination I/O port address
 */
extern "C" void outputsw(natw vectw[], int length, ioaddr reg);

/*
 * Utility functions to read from the keyboard.
 */

/**
 * @return the keycode of the key presssed or released on the keyboard.
 */
natb get_code();

/**
 * @return the ASCII char corresponding to the key pressed on the keyboard, or
 *         0 if the key is not a valid ASCII char.
 */
char char_read();

/**
 * Converts the given keycode to the corresponding ASCII char.
 *
 * @param   k   the keycode to be converted.
 *
 * @return  the ASCII char corresponding to the given keycode.
 */
char keycode_to_ascii(natb k);

/*
 * Utility functions to setup the video ourput.
 */

/**
 *
 * @param   screen_max_x
 * @param   screen_max_y
 */
natb* bochsvga_config(natw screen_max_x, natw screen_max_y);

/*
 * Utility functions to write to the video output.
 */

/**
 * Clears the screen to the given color value.
 *
 * @param   color   the color value to clear the screen with
 */
void clear_screen(natb color);

/**
 * Writes the given char to the video output.
 *
 * @param cha   the char to be written:
 *                1. if cha is equal to '\r' the position of the cursor is set
 *                   to column 0 (same line);
 *                2. if cha is equal to '\n' the position of the cursor is set
 *                   to column 0 on the following line.
 *                3. if cha is equal to '0' nothing happens.
 */
void char_write(natb cha);

/**
 * Writes the given string to the video output (the termination char '\0' is not
 * included).
 *
 * @param   buf[]   the string to be written to the video output.
 */
void str_write(const char buf[]);

/*
 * Utility functions to read and write to the primary hard disk drive.
 */

/**
 * Writes the given data to specified HDD sectors starting from the specified
 * sector.
 *
 * @param   start       starting sector
 * @param   sectors     sectors to be written
 * @param   data[]      data to be written
 */
void hdd_read(natl start, natb sectors, natw data[]);

/**
 * Reads into data from the specified HDD sectors starting from the specified
 * start sector.
 *
 * @param   start       starting sector
 * @param   sectors     sectors to be read
 * @param   data[]      retrieved data
 */
void hdd_write(natl start, natb sectors, natw data[]);

/*
 * Utility functions for the serial ports.
 */

/**
 * Writes the given byte to the COM1 serial interface THR register.
 * 
 * @param   b   the byte to be written
 */
void serial1_o(natb b);

/**
 * Reads a byte from the COM1 serial interface RBR register.
 *
 * @param   &b   the retrieved byte
 */
void serial1_i(natb &b);

/**
 * Write the given byte to the COM2 serial interface THR register.
 *
 * @param   b   the byte to be written
 */
void serial2_o(natb b);

/**
 * Reads a byte from the COM2 serial interface RBR register.
 *
 * @param   &b   the retrieved byte
 */
void serial2_i(natb &b);

/*
 * Utility functions to work with strings.
 */

/**
 * 
 * @param   dest
 * @param   c
 * @param   n
 */
void *memset(void* dest, int c, size_t n);

/**
 *
 * @param   dest
 * @param   src
 * @param   n
 */
void *memcpy(void* dest, const void* src, size_t n);

/**
 * @param   *str   the given string
 *
 * @return  the length of the given string (the termination char '\0' is not
 *          included)
 */
size_t strlen(const char *str);

/**
 * Copies the src string to the dst string.
 *
 * @param   src[]   source string to be copied
 * @param   dst[]   destination string
 * 
 * @return  the address of the ending char '\0' in dst.
 */
char* copy(const char src[], char dst[]);

/**
 * Converts the given natural number n, having the length of a byte, in a string
 * of 2 chars which represent the given hex digits. Copies the resulting string
 * into dst.
 *
 * @param   n       the natural number to be converted
 * @param   dst[]   the string resulting from the conversion
 * 
 * @return  the address of the ending char '\0' in dst (dst + 2).
 */
char* natb_conv(natb n, char dst[]);

/**
 * Converts the given natural number n, having the length of a word, in a string
 * of 4 chars which represent the given hex digits. Copies the resulting string
 * into dst.
 *
 * @param   n       the natural number to be converted
 * @param   dst[]   the string resulting from the conversion
 * 
 * @return  the address of the ending char '\0' in dst (dst + 4).
 */
char* natw_conv(natw n, char dst[]);

/**
 * Converts the given natural number n, having the length of a long, in a string
 * of 8 chars which represent the given hex digits. Copies the resulting string
 * into dst.
 *
 * @param   n       the natural number to be converted
 * @param   dst[]   the string resulting from the conversion
 * 
 * @return  the address of the ending char '\0' in dst (dst + 8).
 */
char* natl_conv(natl n, char dst[]);

/**
 * Converts the given natural number n, having the length of a quad, in a string
 * of 16 chars which represent the given hex digits. Copies the resulting string
 * into dst.
 *
 * @param   n       the natural number to be converted
 * @param   dst[]   the string resulting from the conversion
 * 
 * @return  the address of the ending char '\0' in dst (dst + 16).
 */
char* natq_conv(natq n, char dst[]);

/**
 * Converts the given integer number n in a string of chars which represent the 
 * sign and the hex digits. Copies the resulting string into dst.
 *
 * @param   n       the integer number to be converted
 * @param   dst[]   the string resulting from the conversion
 * 
 * @return  the address of the ending char '\0' in dst.
 */
char* int_conv(long n, char dst[]);

/*
 * Keep in mind that the previous functions can be used to obtain a String of
 * text by concatenation of partial strings of chars, natural and integer
 * numbers: the first function to be called must be given as second actual
 * parameter the first element of the buffer used to contain the final string
 * and the second actual parameter of any successive function called must be the
 * address returned by the previous one.
 */

/*
 * Utility functions to enter a wait state.
 */

/**
 * Prints the string "Press ESC to continue" to the video output and waits for
 * the ESC key to be pressed.
 */
void pause();

/*
 * Utility functions to work with the interrupt mechanism.
 */

/**
 * Initializes the entry with index num of the IDT with the address of the given
 * sub routine.
 *
 * @param   num         IDT entry index
 * @param   routine()   sub routine address
 */
extern "C" void gate_init(natl num, void routine());

/**
 *
 * @param   num
 * @param   routine()
 */
extern "C" void trap_init(natl num, void routine());

/*
 * Utility functions for thread synchronization.
 */

/**
 * Sets the specified semaphore to 0 and waits for it to be set to 1.
 *
 * @param   sem   the semaphore to set
 */
extern "C" void sem_wait(natl sem);

/**
 * Sets the specified semaphore to 1.
 *
 * @param   sem     the semaphore to set
 */
extern "C" void sem_signal(natl sem);

/*
 * Utility functions to implment time delay.
 */

/**
 * Waits for the amount of milliseconds specified by num*50.
 *
 * @param   num     50*num = milliseconds to wait
 */
extern "C" void delay(natl num);

/*
 * Utility functions to work with the PCI bus.
 */

/*
 * Utility functions to work with Heap Memory.
 */

/**
 *
 * @param   v
 * @param   a
 *
 * @return
 */
size_t allinea(size_t v, size_t a);

/**
 * Initializes the heap memory space starting from start with a dimension of
 * 'siize'.
 *
 * @param  start  heap memory space start address;
 * @param  size   heap memory space size.
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
 */
size_t available_heap();

/*
 * Advanced utility functions.
 */

/**
 *
 * @param   fmt
 *
 * @return
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

