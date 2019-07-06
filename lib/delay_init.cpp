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
    gate_init(240, a_delay);
    gate_init(40, a_driver_timer);
    apic_set_VECT(2, 40);		// piedino 0 associato al tipo 40
}
