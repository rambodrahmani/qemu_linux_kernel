//****************************************************************************************************
// file mod_hddr.cpp
#include <libce.h>

const ioaddr iBR = 0x01F0;
const ioaddr iCNL = 0x01F4;
const ioaddr iCNH = 0x01F5;
const ioaddr iSNR = 0x01F3;
const ioaddr iHND = 0x01F6;
const ioaddr iSCR = 0x01F2;
const ioaddr iERR = 0x01F1;
const ioaddr iCMD = 0x01F7;
const ioaddr iSTS = 0x01F7;
const ioaddr iDCR = 0x03F6;
const ioaddr iASR = 0x03F6;

const natl sincr= 7;
natb cont;
natb* punt;

extern "C" void a_go_hddr();
extern "C" void a_driver_hddr();
void ini()
{	gate_init(247, &a_go_hddr);
	gate_init(47, &a_driver_hddr);
	apic_set_VECT(14, 47);
	apic_set_MIRQ(14, false);
}

extern "C" void c_go_hddr(natl indse, natb nn, natb vv[])
{	natb lba_0 = indse,
		lba_1 = indse >> 8,
		lba_2 = indse >> 16,
		lba_3 = indse >> 24;	
	outputb(lba_0, iSNR); 			// indirizzo del settore e selezione drive
	outputb(lba_1, iCNL);
	outputb(lba_2, iCNH);
	natb hnd = (lba_3 & 0x0F) | 0xE0;
	outputb(hnd, iHND);
	outputb(nn, iSCR);				// numero di settori
	outputb(0x00, iDCR);			// abilitazione interruzioni
	outputb(0x20, iCMD);			// comando di lettura
	cont = nn;
	punt = vv;
	sem_wait(sincr);
}

extern "C" void c_driver_hddr()
{	natb stato;
	natw* p = reinterpret_cast<natw*>(punt);
	inputb(iSTS, stato);					 // ack dell'interrupt
	cont--;
	if (cont == 0)
	{	outputb(0x02, iDCR);			//disabilitazione interruzioni
		sem_signal(sincr);
	}
	for (int i = 0; i < 512 / 2; i ++) inputw(iBR, p[i]);
	punt += 512;
}
//****************************************************************************************************
