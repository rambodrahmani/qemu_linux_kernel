/**
 * File: available_heap.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"

size_t available_heap()
{
    size_t tot = 0;

    for (des_mem *scan = free_heap; scan; scan = scan->next)
    {
        tot += scan->size; 
    }

    return tot;
}

