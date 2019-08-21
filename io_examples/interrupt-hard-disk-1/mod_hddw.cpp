//****************************************************************************************************
// file mod_hddw.cpp
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

const natl sincr= 6;
natb cont;
natb* punt;

extern "C" void a_go_hddw();
extern "C" void a_driver_hddw();
void ini()
{	gate_init(246, &a_go_hddw);
	gate_init(46, &a_driver_hddw);
	apic_set_VECT(14, 46);
	apic_set_MIRQ(14, false);
}

extern "C" void c_go_hddw(natl indse, natb nn, natb vv[])
{	natb lba_0 = indse,
		lba_1 = indse >>  8,
		lba_2 = indse >> 16,
		lba_3 = indse >> 24;
	outputb(lba_0, iSNR); 			// indirizzo del settore e selezione drive
	outputb(lba_1, iCNL);
	outputb(lba_2, iCNH);
	natb hnd = (lba_3 & 0x0F) | 0xE0;
	outputb(hnd, iHND);
	outputb(nn, iSCR);			// numero di settori
	outputb(0x00, iDCR);			// abilitazione interruzioni
	outputb(0x30, iCMD);			// comando di scrittura
	cont = nn;
	natw* p = reinterpret_cast<natw*>(vv);
	punt = vv + 512;
	natb s;
	do								// attende che BUSY valga 0 e DREQ valga 1
		inputb(iSTS, s);
	while ((s & 0x88) != 0x08);
	for (int i = 0; i < 512/2; i++) outputw(p[i], iBR);
	sem_wait(sincr);
}

extern "C" void c_driver_hddw()
{	natb stato;
	natw* p = reinterpret_cast<natw*>(punt);
	inputb(iSTS, stato);					 // ack dell'interrupt
	cont--;
	if (cont == 0)
	{	outputb(0x02, iDCR);			//disabilitazione interruzioni
		sem_signal(sincr);
	}
	else for (int i = 0; i < 512/2; i++) outputw(p[i], iBR);
	punt += 512;
}
//****************************************************************************************************
