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
 *         Created on 17/07/2019.
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
    st1 s1 = { 'e', 'b', 'f', 'd' };

    st2 sa = { 1, 20, 3, 40 };

    st2 sb = { 10, 2, 30, 4 };

    cl cla('a', sa);

    cla.stampa();

    cla.elab1(s1, sb);

    cla.stampa();

    return 0;
}

