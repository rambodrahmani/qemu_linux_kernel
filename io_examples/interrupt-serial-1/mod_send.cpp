/**
 * File: mod_send.cpp
 *       Initializes the IDT gates and the APIC pins. Contains implementations
 *       for the 244 and 44 primitives.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/08/2019.
 */

#include <libqlk.h>
#include <COM1.h>
#include <apic.h>
#include <internal.h>

/**
 * Use semaphore 13 for synchronization.
 */
const natl sincr_s = 4;

/**
 * Number of chars to be sent on the serial interface COM1.
 */
natl cont;

/**
 * Memory location address for the chars to be sent.
 */
char* punt;

/**
 * Interrupt 244 primitive.
 */
extern "C" void a_serial_send();

/**
 * Interrupt 44 primitive.
 */
extern "C" void a_serial_driverout();

/**
 * Initializes custom IDT entries and the APIC pins.
 */
void ini()
{
    // Initializes serial interface COM1
    init_COM1();

    // initialize entry 244 of the IDT with a_serial_send address
    gate_init(244, &a_serial_send);

    // initialize entry 44 of the IDT with a_serial_driverout address
    gate_init(44, &a_serial_driverout);

    // set interrupt 44 for APIC pin 4
    apic_set_VECT(4, 44);

    // enable APIC pin number 4 interrupt requests
    apic_set_MIRQ(4, false);
}

/**
 * C++ implementation for a_serial_send body.
 */
extern "C" void c_serial_send(natl nn, char vv[])
{
    // set number of chars to be sent
    cont = nn; 

    // set memory location address
    punt = vv;

    // enable serial interface COM1 output interrupt requests
    outputb(0x02, iIER);

    // wait for semapgore sincr_s
    sem_wait(sincr_s);
}

/**
 * Called everytime the serial interface COM1 sends an interrupt reuqest.
 */
extern "C" void c_driverout_serial()
{
    // decrease chars counter
    cont--;

    // if all chars have been sent
    if (cont == 0)
    {
        // disable serial interface COM1 interrupt requests
        outputb(0x00, iIER);

        // signal synchronization semaphore
        sem_signal(sincr_s);
    }

    // retrieve ASCII char from memory
    natb a;
    a = *punt;

    // write char to the RBR register
    outputb(a, iRBR);

    // increase memory pointer
    punt++;
}

