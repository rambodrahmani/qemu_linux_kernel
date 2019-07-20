/**
 * File: init_keyboard.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>
#include <apic.h>
#include <kbd.h>

// si usa il semaforo numero 1
const natl sincr_t = 1;
natl cont;
char* punt;
extern "C" void a_leggi_linea();
extern "C" void a_driv_tasint();
void ini()
{
	gate_init(241, a_leggi_linea);
	gate_init(41, a_driv_tasint);
	apic_set_VECT(1, 41);
	apic_set_MIRQ(1, false);
}
extern "C" void c_leggi_linea(int& nn, char vv[])
{
	int old_nn = nn;
	cont = nn;
	punt = vv;
        // abilit. richieste interruzione tastiera
	outputb(0x60, iCMR);
	outputb(0x61, iTBR);
	sem_wait(sincr_t);
	nn = old_nn-cont;
}
natb get_code_int()
{
	natb c;
	inputb(iRBR, c);
	return c;
}
extern bool shift;
extern "C" void c_driv_tasint()
{
	natb a;
        // disab. richieste di interruzione tastiera
	outputb(0x60, iCMR);
	outputb(0x60, iTBR);
	a = get_code_int();
	if (a == 0x2A)      // left shift make code
		shift = true;
	else if (a == 0xAA) // left shift break code
		shift = false;
	a = keycode_to_ascii(a);
	if (a == 0) {
		// riab. richieste di interruzione tastiera
		outputb(0x60, iCMR);
		outputb(0x61, iTBR);
		return;
	}
	cont--;
	*punt = a;
	char_write(a);
	if ((a == '\n') || (cont == 0)) {
                // ultimo carattere trasferito
		sem_signal(sincr_t);
		return;
	}
	punt++;
        // riab. richieste di interruzione tastiera
	outputb(0x60, iCMR);
	outputb(0x61, iTBR);
}
//****************************************************************************
