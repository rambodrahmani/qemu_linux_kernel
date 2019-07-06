/**
 * File: c_delay.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "delay.h"

extern "C" void c_delay(natl num)
{
    num_t = num;
    apic_set_MIRQ(2, false);	// abilit. piedino 0 dello APIC
    sem_wait(sincr);
}
