//****************************************************************************************************
// file mod_ric.cpp
#include <libce.h>

// COM1, collegata al piedino 4 dello IO APIC
// ingresso dati a interruzione di programma

const ioaddr iRBR = 0x03F8;			// D_LAB deve essere 0
const ioaddr iTHR = 0x03F8; 			// D_LAB deve essere 0
const ioaddr iLSR = 0x03FD;
const ioaddr iLCR = 0x03FB;
const ioaddr iDLR_LSB = 0x03F8;			// D_LA B deve essere 1
const ioaddr iDLR_MSB = 0x03F9; 		// D_LA B deve essere 1
const ioaddr iIER = 0x03F9; 			// D_LAB deve essere 0
const ioaddr iIIR = 0x03FA;
const ioaddr iMCR = 0x03FC;


const natl sincr_s = 12;			// si usa il semaforo numero 12
natl cont;
char* punt;

void ini_ip_COM1()
{	natw CBITR = 0x000C;
	natb dummy;
	outputb(0x80, iLCR);			// D_LAB a 1
	outputb(CBITR, iDLR_LSB);		// bit rate 9600
	outputb(CBITR >> 0x08, iDLR_MSB);
	outputb(0x03, iLCR);			// 1 bit stop, 8 bit/car, D_LAB a 0
	outputb(0x00, iIER);			// disab. richieste interruzione
	inputb(iRBR, dummy);
	outputb(0x08, iMCR);			// abilitazione MCR
}

extern "C" void a_ricevi_serial();
extern "C" void a_driverin_serial();
void ini()
{	ini_ip_COM1();
	gate_init(245, &a_ricevi_serial);
	gate_init(45, &a_driverin_serial);
	apic_set_VECT(4, 45); 
	apic_set_MIRQ(4, false);		// abilit. piedino n. 4 dello IO APIC 
}

extern "C" void c_ricevi_serial(natl nn, char vv[])
{	cont = nn; 
	punt = vv;
	outputb(0x01, iIER);			// abilit. richieste di interruzione ingresso
	sem_wait(sincr_s);
}

extern "C" void c_driverin_serial()
{	natb a;
	cont--;
	if (cont == 0)
	{	outputb(0x00, iIER); 		// disab. richieste di interruzione
		sem_signal(sincr_s);
	}
	inputb(iRBR, a);
	*punt = a; 				// trasferimento in memoria
	punt++;
}
//****************************************************************************************************
