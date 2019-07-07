/**
 * File: delay_init.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "delay.h"

extern "C" void delay_init()
{
    ini_ip_cont_0();

    // initialize IDT entry number 240 with a_delay subroutine address
    gate_init(240, a_delay);

    // initialize IDT entry number 40 with a_driver_timer subroutine address
    gate_init(40, a_driver_timer);

    // set type 40 for the interrupts coming from pin number 2
    apic_set_VECT(2, 40);
}

