/**
 * File: es1.cpp
 *       Contains the C++ code to be translated into Assembly (es1.s file).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 14/09/2019.
 */

#include "cc.h"

cl::cl(char c, st2& s2)
{
    for (int i = 0; i < 4; i++)
    {
        s.vc[i] = c + i;
        v[i] = s2.vd[i] + s.vc[i];
    }
}

void cl::elab1(st1 s1, st2 s2)
{
    cl cla('a', s2);
    
    for (int i = 0; i < 4; i++)
    {
        if (s.vc[i] <= s1.vc[i])
        {
            s.vc[i] = cla.s.vc[i];
            v[i] = cla.v[i];
        }
    }
}

