/**
 * File: apic_in.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

natl apic_in(natb off)
{
    *IOREGSEL = off;
    return *IOWIN;
}
