/**
 * File: type.h
 *       Native types declaration.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 30/08/2019.
 */

#ifndef TIPO_H_
#define TIPO_H_

/**
 * Virtual and physical address in the memory space.
 */
typedef void* addr;

/**
 * Physical address in the I/O space.
 */
typedef unsigned short ioaddr;

/**
 * Native types.
 */
typedef unsigned char  natb;    // 1 byte
typedef unsigned short natw;    // 2 byte
typedef unsigned int   natl;    // 4 byte
typedef unsigned long  natq;    // 8 byte
typedef void* str;
typedef const void* cstr;

#endif

