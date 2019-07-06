/**
 * File: apic_send_EOI.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "internal.h"
#include "apic.h"

extern "C" void apic_send_EOI()
{
    *IOAPIC_EOIR = 0;
}       
