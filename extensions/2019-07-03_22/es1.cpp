#include "cc.h"
cl::cl(char v[])
{	
	for (int i = 0; i < 4; i++) {
		s.vv1[i] = s.vv2[i] = v[i];
	}
}
void cl::elab1(int d, st& ss)
{	
	for (int i = 0; i < 4; i++) {
		if (d >= ss.vv2[i])
			s.vv1[i] += ss.vv1[i]; 
	  	s.vv2[i] = d + i;
	}
}
