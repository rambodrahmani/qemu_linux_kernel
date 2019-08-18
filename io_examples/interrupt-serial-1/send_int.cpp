/**
 * File: send_int.cpp
 *       In this I/O example we will show how to use the serial interface using
 *       the interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 18/08/2019.
 */

#include <libce.h>

/**
 *
 */
char buffi[80];

/**
 *
 */
extern "C" void invia_serial(natl nn, char vv[]);

/**
 * Developer harness test shows
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // max 80
    natl quanti = 25;

    char s[] = "I calcolatori elettronici";

    for (int i = 0; i < quanti; i++) buffi[i] = s[i];

    // delay(20); 

    invia_serial(quanti, buffi);

    str_write("OK\n");

    pause();

    return 0;
}

