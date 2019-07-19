/**
 * File: type.h
 *       Native types declaration.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019.
 */

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

/**
 * size_t definition based on the cpu architecture.
 */
#ifdef x86_64
    typedef unsigned long size_t;
#else
    typedef unsigned int size_t;
#endif

