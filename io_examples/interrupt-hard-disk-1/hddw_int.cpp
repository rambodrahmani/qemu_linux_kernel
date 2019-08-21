/**
 * File: mod_send.cpp
 *       Initializes the IDT gates and the APIC pins. Contains implementations
 *       for the 244 and 44 primitives.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/08/2019.
 */

#include <libce.h>

natb buffw[8*512];

extern "C" void go_hddw(natl indse, natb nn, natb vv[]);
int main()
{	natl lba, quanti;
	lba = 1;
	quanti = 3;						// massimo 8
	for (int i = 0; i < quanti*512; i++) buffw[i] ='i';
	go_hddw(lba, quanti, buffw);
	str_write("OK\n");
	pause();
	return 0;
}

