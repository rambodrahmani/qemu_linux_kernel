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
 *         Created on 19/07/2019.
 */

#include "cc.h"

/**
 * Developer harness test.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    st1 s1 = { 1 , 2 , 3 , 4 };
     
    st2 s2 = { 5 , 6 ,7 , 8 };
    
    char a1[4] = { 11,12,13,14 };
    
    int a2[4] = {15,16,17,18 };
    
    cl cla1(s1);
    
    cla1.stampa();
    
    cl cla2(s1, a2);
    
    cla2.stampa();
    
    //cla1 = cla2.elab1(a1, s2);
    
    //cla1.stampa();
    
    //system("pause");
    
    return 0;
}

