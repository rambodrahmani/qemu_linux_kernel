/**
 * File: ini_ip_cont_0.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "delay.h"

void ini_ip_cont_0()
{
    natl N = 59500;                  // costante di tempo relativa a 50 ms
    outputb(0x36, iCWR_0);           // contatore 0 in modo 3
    outputb(N, iCTR0_LSB);
    outputb(N >> 8, iCTR0_MSB);
}
