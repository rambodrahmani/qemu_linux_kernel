/**
 * File: c_driver_timer.cpp
 *       C++ implementation for a_driver_timer.s.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "apic.h"
#include "delay.h"

extern "C" void c_driver_timer()
{
    num_t--;

    if (num_t == 0)
    {
        // disable APIC ir pin 2
        apic_set_MIRQ(2, true);

        // wait for semaphore sincr
        sem_signal(sincr);
    }
}

