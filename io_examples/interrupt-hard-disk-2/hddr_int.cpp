//****************************************************************************************************
// file hddr_int.cpp
#include <libce.h>

natb buffr[8*512];

extern "C" void go_hddr(natl indse, natb nn, natb vv[]);
int main()
{	char c;
	natl lba, quanti; 
	lba = 1;
	quanti = 3;							// massimo 8
	go_hddr(lba, quanti, buffr);
	for (int i = 0; i < quanti*512; i++) char_write(buffr[i]);
	char_write('\n');
	pause();
	return 0;
}
//****************************************************************************************************
