/**
 * File: apic_out.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

void apic_out(natb off, natl v)
{
    *IOREGSEL = off;
    *IOWIN = v;
}
