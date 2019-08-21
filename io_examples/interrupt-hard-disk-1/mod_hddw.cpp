/**
 * File: mod_hddw.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 21/08/2019.
 */

#include <libqlk.h>
#include <hdd.h>
#include <apic.h>

/**
 *
 */
const natl sincr = 6;

/**
 *
 */
natb cont;

/**
 *
 */
natb* punt;

/**
 * Interrupt 246 primitive.
 */
extern "C" void a_go_hddw();

/**
 * Interrupt 46 primitive.
 */
extern "C" void a_driver_hddw();

/**
 * Initializes custom IDT entries and the APIC pins.
 */
void ini()
{
    // initialize entry 246 of the IDT with the a_go_hddw address
    gate_init(246, &a_go_hddw);

    // initialize entry 46 of the IDT with the a_driver_hddw address
    gate_init(46, &a_driver_hddw);

    // set interrupt 46 for APIC pin 14
    apic_set_VECT(14, 46);

    // enable APIC ping 14 interrupt requests
    apic_set_MIRQ(14, false);
}

/**
 *
 */
extern "C" void c_go_hddw(natl indse, natb nn, natb vv[])
{
    //
    natb lba_0 = indse,
         lba_1 = indse >>  8,
         lba_2 = indse >> 16,
         lba_3 = indse >> 24;

    //
    outputb(lba_0, iSNR); 			// indirizzo del settore e selezione drive

    //
    outputb(lba_1, iCNL);

    //
    outputb(lba_2, iCNH);

    //
    natb hnd = (lba_3 & 0x0F) | 0xE0;

    //
    outputb(hnd, iHND);

    //
    outputb(nn, iSCR);			// numero di settori

    //
    outputb(0x00, iDCR);			// abilitazione interruzioni

    //
    outputb(0x30, iCMD);			// comando di scrittura

    //
    cont = nn;

    //
    natw* p = reinterpret_cast<natw*>(vv);

    //
    punt = vv + 512;

    //
    natb s;

    //
    do								// attende che BUSY valga 0 e DREQ valga 1
    {
        inputb(iSTS, s);
    } while ((s & 0x88) != 0x08);

    //
    for (int i = 0; i < 512/2; i++) outputw(p[i], iBR);

    //
    sem_wait(sincr);
}

/**
 *
 */
extern "C" void c_driver_hddw()
{
    natb stato;

    natw* p = reinterpret_cast<natw*>(punt);

    inputb(iSTS, stato);					 // ack dell'interrupt

    cont--;

    if (cont == 0)
    {
        outputb(0x02, iDCR);			//disabilitazione interruzioni
        sem_signal(sincr);
    }
    else
    {
        for (int i = 0; i < 512/2; i++)
        {
            outputw(p[i], iBR);
        }
    }

    punt += 512;
}

