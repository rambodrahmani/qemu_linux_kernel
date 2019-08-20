/**
 * File: send_int.cpp
 *       In this I/O example we will show how to use the serial interface using
 *       the interrupt mechanism.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 20/08/2019.
 */

#include <libqlk.h>
#include <COM1.h>
#include <apic.h>
#include <internal.h>

/**
 * Use semaphore number 12 for synchronization.
 */
const natl sincr_s = 12;

/**
 * Number of chars to be retrived by the driver.
 */
natl cont;

/**
 * Memory location pointer where to store the retrieved chars.
 */
char* punt;

/**
 * Interrupt 245 primitive.
 */
extern "C" void a_serial_receive();

/**
 * Interrupt 45 primitive.
 */
extern "C" void a_serial_driverin();

/**
 * Initializes custom IDT entries and the APIC pins.
 */
void ini()
{
    // Initializes serial interface COM1
    init_COM1();

    // initialize entry 245 of the IDT with a_serial_receive address
    gate_init(245, &a_serial_receive);

    // initialize entry 45 of the IDT with a_serial_driverin address
    gate_init(45, &a_serial_driverin);

    // set interrupt 45 for APIC pin 4
    apic_set_VECT(4, 45);

    // enable APIC pin number 4 interrupt requests
    apic_set_MIRQ(4, false);
}

/**
 * C++ body implementation for a_serial_receive.
 */
extern "C" void c_serial_receive(natl nn, char vv[])
{
    // set number of chars to receive on serial interface COM1
    cont = nn;

    // set memory location address
    punt = vv;

    // enable serial interface COM1 interrupt requests
    outputb(0x01, iIER);

    // wait for semaphore sincr_s
    sem_wait(sincr_s);
}

/**
 * Called everytime the serial interface COM1 sends an interrupt reuqest.
 */
extern "C" void c_serial_driverin()
{
    // decrease received chars counter
    cont--;

    // if all chars have been received
    if (cont == 0)
    {
        // disable serial interface COM1 interrupt requests
        outputb(0x00, iIER);

        // signal synchronization semaphore
        sem_signal(sincr_s);
    }

    // retrive a byte from serial interface COM1 RBR register
    natb a;
    inputb(iRBR, a);

    // write retrived byte to memory
    *punt = a;

    // increase memory location pointer address
    punt++;
}

