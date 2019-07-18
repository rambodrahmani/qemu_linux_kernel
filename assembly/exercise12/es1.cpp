/**
 * File: es1.cpp
 *       Contains the C++ code to be translated into Assembly (es1.s file).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */
#include "cc.h"

cl::cl(st1 ss)
{
    for (int i = 0; i < 4; i++)
    {
        v1[i] = ss.vi[i];
        v2[i] = ss.vi[i] * 2;
        v3[i] = 2 * ss.vi[i];
    }
}

cl::cl(st1 & s1, int ar2[])
{	
    for (int i = 0; i < 4; i++)
    {
        v1[i] = s1.vi[i];
        v2[i] = s1.vi[i] * 4;
        v3[i] = ar2[i];
    }
}

cl cl::elab1(char ar1[], st2 s2)
{	
    st1 s1;

    for (int i = 0; i < 4; i++)
    {
        s1.vi[i] = ar1[i] + i;
    }

    cl cla(s1);

    for (int i = 0; i < 4; i++)
    {
        cla.v3[i] = s2.vd[i];
    }

    return cla;
}

