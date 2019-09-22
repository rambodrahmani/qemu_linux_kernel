/**
 * File: prova1.cpp
 *       This file contains a developer harness test for es1.s.
 *
 *       Compile with:
 *          g++ -o es1 -fno-elide-constructors es1.s prova1.cpp
 *
 *       Test your result with:
 *          ./es1 | diff - es1.out
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 14/09/2019.
 */

#include "cc.h"

/**
 * Developer harness test.
 *
 * @param  argc  command line arguments counter.
 * @param  argv  command line arguments.
 *
 * @return       execution exit code.
 */
int main(int argc, char * argv[])
{
    st s = { 1, 2, 3, 4, 1, 2, 3, 4 };

    char v[4] = { 10, 11, 12, 13 };

    int d = 2;

    cl cc1(v);

    cc1.stampa();

    cc1.elab1(s, d);

    cc1.stampa();
}

