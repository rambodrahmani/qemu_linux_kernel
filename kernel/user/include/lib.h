/**
 * File: lib.h
 *       User Module library. The utility methods declared here are defined in
 *       lib.cpp and will be executed at user level.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/09/2019.
 */

#ifndef _LIB_H_
#define _LIB_H_

#include <sys.h>

/**
 *
 */
void *mem_alloc(natl quanti);

/**
 *
 */
void mem_free(void *ptr);

/**
 *
 */
int printf(const char *fmt, ...);

/**
 *
 */
int sprintf(char *buf, natq size, const char *fmt, ...);

/**
 *
 */
void pause();

#endif

