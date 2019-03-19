/**
 *
 * File: type.h
 *       Native types declaration.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/03/2019
 *
 */

// virtual and physical addresses in the memory space
typedef void* addr;

// physical address in the I/O space
typedef unsigned short ioaddr;

// native types declaration
typedef unsigned char natb;
typedef unsigned short natw;
typedef unsigned int natl;
typedef unsigned long natq;
typedef void* str;
typedef const void* cstr;

#ifdef x86_64
    typedef unsigned long size_t;
#else
    typedef unsigned int size_t;
#endif

