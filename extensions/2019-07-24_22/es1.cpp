/**
 * File: es1.cpp
 *       Contains the C++ code to be translated into Assembly (es1.s file).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 14/09/2019.
 */

#include "cc.h"

cl::cl(char v[])
{
    for (int i = 0; i < 4; i++)
    {
        s.vv1[i] = s.vv2[i] = v[3 - i];
    }
}

void cl::elab1(st& ss, int d)
{	
    for (int i = 0; i < 4; i++)
    {
        if (d >= ss.vv2[i])
        {
            s.vv1[i] += ss.vv1[i]; 
        }

        s.vv2[i] = d - i;
    }
}

