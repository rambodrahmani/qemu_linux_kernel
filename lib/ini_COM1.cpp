/**
 * File: ini_COM1.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include "libqlk.h"
#include "COM1.h"

void ini_COM1()
{
    natw CBITR = 0x000C;		// 9600 bit/sec.
    natb dummy;
    
    outputb(0x80, iLCR);		// DLAB 1
    outputb(CBITR, iDLR_LSB);
    outputb(CBITR >> 8, iDLR_MSB);
    outputb(0x03, iLCR);		// 1 bit STOP, 8 bit/car, parita dis, DLAB 0
    outputb(0x00, iIER);		// richieste di interruzione disabilitate

    inputb(iRBR, dummy);		// svuotamento buffer RBR
}
