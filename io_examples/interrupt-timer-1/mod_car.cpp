/**
 * File: mod_car.cpp
 *       The counter interface 8254 we worked with in previous examples can also
 *       be controlled using the interrupt mechanism. The OUT0 pin of the
 *       counter n. 0 is connected to the pin IR0 of the APIC.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 18/08/2019.
 */

#include <libqlk.h>
#include <apic.h>

/**
 * Mode/Command register I/O port address.
 */
const ioaddr iCWR = 0x43;

/**
 * Channel 0 data I/O port address (LSB).
 */
const ioaddr iCTR0_LSB = 0x40;

/**
 * Channel 0 data I/O port address (MSB).
 */
const ioaddr iCTR0_MSB = 0x40;

/**
 * We will be using semaphore number 2 for synchronization.
 */
const natl sincr_t = 2;

/**
 *
 */
natl num_t;

/**
 * Initialize counter 0 with time constant T = N/Tc = 59500/(1190 * 10^3) =
 * 50ms, in mode 3.
 */
void ini_ip_CONT0()
{
    // conuter time constant
    natl N = 59500;

    // initialize counter 0 in mode 3: infinite loop, software trigger with
    // square wave output
    outputb(0x36, iCWR);
    outputb(N, iCTR0_LSB);
    outputb(N >> 8, iCTR0_MSB);
}

// indirizzo da porre nel gate n. 240
extern "C" void a_go_timer();

// indirizzo da porre nel gate  n. 40
extern "C" void a_driver_timer();

/**
 * Initializes custom IDT entries and the APIC pins.
 */
void ini()
{
    // initialize IDT entry 242 with the a_go_timer address
    gate_init(242, a_go_timer);

    // initialize IDT entry 42 with the a_driver_timer address
    gate_init(42, a_driver_timer);

    // set interrupt type for APIC pin number 0
    apic_set_VECT(0, 42);

    // initialize counter 0
    ini_ip_CONT0();
}

/**
 * go_timer() primitive C++ body implementation.
 */
extern "C" void c_go_timer(natl num)
{
    // set the number of 'D' chars to be printed
    num_t = num;

    // enable APIC pin number 0 to send interrupt requests
    apic_set_MIRQ(0, false);

    // wait for semaphore sincr_t
    sem_wait(sincr_t);
}

/**
 * Every 50ms this driver will be executed.
 */
extern "C" void c_driver_timer()
{
    // write 'D' char to the video output
    char_write('D');

    // decrease written chars counter
    num_t--;

    // check if num_t chars have been written to the video output
    if (num_t == 0)
    {
        // disable APIC pin number 0 interrupt requests
        apic_set_MIRQ(0, true);

        // signal semaphore sincr_t
        sem_signal(sincr_t);
	}
}

