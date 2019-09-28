/**
 * File: heap_init.cpp
 *       Initializes the heap memory space with the given parameters.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"

/**
 * @param  start  heap memory area starting address;
 * @param  size   heap memory area size.
 */
void heap_init(void *start, size_t size)
{
    internal_free(start, size);
}

