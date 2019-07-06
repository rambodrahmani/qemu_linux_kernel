/**
 * File: heap_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"

void heap_init(void *start, size_t size)
{
    free_interna(start, size);
}
