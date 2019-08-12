/**
 * File: keyboard-2-interrupt-rev0.cpp
 *       Very first implementation for keyboard-2-interrupt.cpp.
 *       In this example we modify keyboard-1-interrupt.cpp previously developed
 *       in order to show how to work with multiple interrupt requests sources.
 *
 *       Compile using:
 *          ./compile
 *
 *       Execute using:
 *          ./boot
 *
 *       Output:
 *          When the program is executed, we can see the spinning bar on top of
 *          the video output and the keycodes being printed when the driver
 *          kicks in. In revision 1 we will be doing some changes to this first
 *          implementation in order to show how the parallel execution of the
 *          main program and the driver.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 11/08/2019.
 */

#include <libqlk.h>
#include <kbd.h>
#include <apic.h>
#include <vid.h>

/**
 * In order to show the main program execution flow visible, we display a
 * spinner on top of the video output. The spinning effect is obtained by
 * continuously overwrinting a position in the video matrix with chars that
 * resembly a rotating bar.
 */
char spinner[] = {'|', '/', '-', '\\'};

/**
 * Set to true when the ESC keycode is retrieved.
 */
volatile bool stop = false;

/**
 * Assembly Keyboard driver.
 */
extern "C" void a_keyboard_driver();

/**
 * C++ Keyboard driver. [0]
 */
extern "C" void c_keyboard_driver()
{
    // temp variable, can't pass a volatile variable as reference
    natb c;

    // read a byte from the RBR register to c
    inputb(iRBR, c);

    // check for the ESC key
    if (c == 0x01)
    {
        stop = true;
    }

    // write binary keycode to the video output
    for(int i = 0; i < 8; i++)
    {
        // write '1' or '0'
        char_write(c & 0x80 ? '1':'0');

        // shift left the keycode
        c <<= 1;
    }

    // write a new line to the video output
    char_write('\n');

    // send EOI
    apic_send_EOI();
}

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard using
 * the interrupt mechanims.
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // initialize IDT entry 40 with the a_driver address, [1]
    gate_init(0x40, a_keyboard_driver);

    // set interrupt type for APIC pin number 1
    apic_set_VECT(1, 0x40);

    // enable APIC pin number 1 to send interrupt requests
    apic_set_MIRQ(1, false);

    //
    apic_set_TRGM(1, true);

    // initialize the keyboard with interrupt requests enabled
    outputb(0x60, iCMR);
    outputb(0x61, iTBR);

    // keep the spinner spinning until stop
    int i = 0;
    while (!stop)
    {
        video[40] = attr << 8 | spinner[i];

        i = (i + 1) % sizeof(spinner);

        // slow down spinner
        for (int k = 0; k < 10000000; k++) {}
    }

    return 0;
}

/**
 * [0]
 * In this upgraded version we moved the retrieving and processsing of the
 * keycodes to the C++ driver. This results in a series of simplifications as
 * the communication between the driver and the main program is now limited to
 * checking for the ESC key only. To be more precise, we don't need the new_char
 * anymore to be shared between the main program and the driver. Additionally
 * the main program can accept interrupt requests at any given time.
 */

/**
 * [1]
 * It is important to outline that we define the type for the interrupt using a
 * hex value (0x40). This makes the definition of the priority class and the
 * sub-prioprity class implicitly defined: 4 for the priority and 0 for the
 * sub-priority.
 * When interrupts are pending in the IRR register, the local APIC dispatches
 * them to the processor one at a time,based on their priority.
 *
 * Each interrupt delivered to the processor through the local APIC has a
 * priority based on its vector number. The local APIC uses this priority
 * to determine when to service the interrupt relative to the other
 * activities of the processor, including the servicing of other
 * interrupts. Each interrupt vector is an 8-bit value. The interrupt-
 * priority class is the value of bits 7:4 of the interrupt vector. The
 * lowest interrupt-priority class is 1 and the highest is 15; interrupts
 * with vectors in the range 0–15 (with interrupt-priority class 0) are
 * illegal and are never delivered. Because vectors 0–31 are reserved for
 * dedicated uses by the Intel 64 and IA-32 architectures, software should
 * configure interrupt vectors to use interrupt-priority classes inthe
 * range 2–15. Each interrupt-priority class encompasses 16 vectors. The
 * relative priority of interrupts within an interrupt-priority class is
 * determined by the value of bits 3:0 of the vector number. The higher
 * the value of those bits, the higher thepriority within that interrupt-
 * priority class. Thus, each interrupt vector comprises two parts, with
 * the high 4 bitsindicating its interrupt-priority class and the low 4
 * bits indicating its ranking within the interrupt-priority class.
 */

