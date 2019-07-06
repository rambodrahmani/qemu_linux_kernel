/**
 * File: c_driver_timer.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "delay.h"

extern "C" void c_driver_timer()
{
    num_t--;

    if (num_t == 0)
    {	
        apic_set_MIRQ(2, true);   // disab. piedino 0 dello APIC
        sem_signal(sincr);
    }
}
