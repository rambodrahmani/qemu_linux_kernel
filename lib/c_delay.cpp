/**
 * File: c_delay.cpp
 *       C++ implementation for a_delay.s.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "apic.h"
#include "delay.h"

extern "C" void c_delay(natl num)
{
    num_t = num;

    // disable APIC ir pin 2
    apic_set_MIRQ(2, false);

    // wait for semaphore sincr
    sem_wait(sincr);
}

