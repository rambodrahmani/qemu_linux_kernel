/**
 * File: es1.cpp
 *       Contains the C++ code to be translated into Assembly (es1.s file).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 26/09/2019.
 */

#include "cc.h"

cl::cl(char c, st2& s2)
{
    for (int i = 0; i < 4; i++)
    {
        c1.vc[i] = c;
        c2.vc[i] = c++;
        v[i] = s2.vd[i] + c2.vc[i];
    }
}

void cl::elab1(st1 s1, st2 s2)
{
    cl cla('a', s2);

    for (int i = 0; i < 4; i++)
    {
        if (c2.vc[i] <= s1.vc[i])
        {
            c1.vc[i] = i + cla.c2.vc[i];
            v[i] = i - cla.v[i];
        }
    }
}

