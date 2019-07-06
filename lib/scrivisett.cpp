#include "internal.h"
#include "hd.h"

void scrivisett(natl lba, natb quanti, natw vetto[])
{	
	natb lba_0 = lba,
		lba_1 = lba >> 8,
		lba_2 = lba >> 16,
		lba_3 = lba >> 24;
	natb s;
	do 
		inputb(iSTS, s);
	while (s & 0x80);
	outputb(lba_0, iSNR);					// indirizzo del settore e selezione drive
	outputb(lba_1, iCNL);
	outputb(lba_2, iCNH);
	natb hnd = (lba_3 & 0x0F) | 0xE0;
	outputb(hnd, iHND);
	outputb(quanti, iSCR);					// numero di settori
	outputb(0x0A, iDCR);					// disabilitazione richieste di interruzione
	outputb(0x30, iCMD);					// comando di scrittura
	for (int i = 0; i < quanti; i++) {
		do
			inputb(iSTS, s);
		while ((s & 0x88) != 0x08);
		for (int j = 0; j < 512/2; j++)
			outputw(vetto[i*512/2 + j], iBR);
	}
}
