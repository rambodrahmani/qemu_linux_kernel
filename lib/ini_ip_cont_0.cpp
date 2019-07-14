/**
 * File: ini_ip_cont_0.cpp
 *       Initializes counter 0 in mode 3.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "delay.h"

void ini_ip_cont_0()
{
    // 50 ms time constant
    natl N = 59500;

    // channel 0 in mode 3
    outputb(0x36, iCWR_0);

    // write time constant LSB
    outputb(N, iCTR0_LSB);

    // write time constant MSB
    outputb(N >> 8, iCTR0_MSB);
}

