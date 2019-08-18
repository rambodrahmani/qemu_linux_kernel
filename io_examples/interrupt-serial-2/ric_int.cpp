//****************************************************************************************************
// file ric_int.cpp
#include <libce.h>

char buffr[80];

extern "C" void ricevi_serial(natl nn, char vv[]);
int main()
{	char c;
	natl quanti;
	quanti = 25;			// massimo 80
	ricevi_serial(quanti, buffr);
	for (int i = 0; i < quanti; i++) char_write(buffr[i]);
	char_write('\n');
	pause();
	return 0;
}
//****************************************************************************************************
