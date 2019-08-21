/**
 * File: disponibile.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "heap.h"

size_t disponibile()
{
    size_t tot = 0;

    for (des_mem *scan = memlibera; scan; scan = scan->next)
    {
        tot += scan->dimensione;                                                                
    }

    return tot;
}

