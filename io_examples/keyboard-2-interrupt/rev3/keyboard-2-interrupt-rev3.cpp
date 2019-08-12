/**
 * File: keyboard-2-interrupt-rev3.cpp
 *       Revision 3.
 *       In this revision we will be using trap_init() instead of gate_init() in
 *       order to initialize the IDT gate for the keyboard interrupt requests so
 *       that the interrupt requests won't be disabled while executing
 *       c_keyboard_driver().
 *
 *       Compile using:
 *          ./compile
 *
 *       Execute using:
 *          ./boot
 *
 *       Output:
 *          When a key is pressed while executing this implementation, we will
 *          notice that the spinner on the left 
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 11/08/2019.
 */

#include <libqlk.h>
#include <kbd.h>
#include <apic.h>
#include <vid.h>
#include <delay.h>

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

        // slow down
        volatile int j;
        for (j = 0; j < 10000000; j++) {}
    }

    // write a new line to the video output
    char_write('\n');

    // send EOI
    apic_send_EOI();
}

/**
 * Assembly Counter driver.
 */
extern "C" void a_timer_driver();

/**
 * C++ Counter driver.
 */
extern "C" void c_timer_driver()
{
    static int i = 0;

    video[50] = attr << 8 | spinner[i];

    i = (i + 1) % sizeof(spinner);

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
    // initialize IDT entry 40 with the a_keyboard_driver address
    trap_init(0x40, a_keyboard_driver);

    // set interrupt type for APIC pin number 1, [1]
    apic_set_VECT(1, 0x40);

    // enable APIC pin number 1 to send interrupt requests
    apic_set_MIRQ(1, false);

    // set trigger mode for the keyboard requests: IO-APIC-level
    apic_set_TRGM(1, true);

    // initialize the keyboard with interrupt requests enabled
    outputb(0x60, iCMR);
    outputb(0x61, iTBR);

    // initialize IDT entry 50 with the a_timer_driver address
    gate_init(0x50, a_timer_driver);

    // set interrupt type for APIC pin number 2
    apic_set_VECT(2, 0x50);

    // enable APIC pin number 2 to send interrupt requests
    apic_set_MIRQ(2, false);

    // set trigger mode for the timer requests: IO-APIC-edge, [2]
    apic_set_TRGM(2, false);

    // initialize counter 0 in mode 3: software triggered, infinite loop,
    // impulse generated at the end of counting every 50ms
    outputb(0x36, iCWR_0);
    outputb((natb)50000, iCTR0_LSB);
    outputb((natb)(50000 >> 8), iCTR0_MSB);

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

/**
 * [2]
 * We will be using egde trigger mode for the counter because, opposed to other
 * interrupt requests sources, the counter interface disables interrupt requests
 * automatically and not like other sources which remove requests when an action
 * (e.g. reading from the RBR register) is performed by the driver.
 */

/**
 * [3]
 * This is what is going on in detail:
 *  1. while we press keys on the keyboard, the main program is spinning the
 *     spinner on the left and, every 50ms, the counter will send an interrupt
 *     request which will make the spinner on the right step too. Everything
 *     works just fine and very smoothly although every step taken by the
 *     spinner on the right means that the CPU is actually jumping to and
 *     executing the timer driver;
 *  2. when a key is pressed on the keyboard, the keyboard controller send the
 *     interrupt requests which will be regisyered by the APIC in the IRR, bit
 *     0x40, and will forward it to the CPU. Sooner or later the CPU will accept
 *     the interrupt request, jump to c_keyboard_driver, disable interrupt
 *     requests, move bit 0x40 from IRR to ISR;
 *  3. for the entire execution time of c_keyboard_driver(), the timer will keep
 *     sending interrupt requests and store them in IRR, bit 0x50 and, since it
 *     has higher priority than the ones coming from the keyboard, will try and
 *     forward to the CPU which will ignore them because interrupt are disabled;
 *  4. when we release the key, the keyboard will send another interrupt
 *     request, probably when c_keyboard_driver is still running, for the break
 *     code; this request too can't be accepted by the CPU too and will remain
 *     in IRR;
 *  5. when the apic_send_EOI() of c_keyboard_driver() is executed, APIC will
 *     reset bit 0x40 of the ISR and will find in IRR two requests: the 0x40 and
 *     the 0x50; APIC will forward the 0x50 to the CPU, which has higher
 *     priority class;
 *  6. the CPU will accept the request when it reaches the iretq of
 *     a_keyboard_driver, jumps to a_timer_driver which will result in a single
 *     step for the spinner on the right;
 *  7. whe CPU executes the apic_send_EOI() in c_timer_driver() it is very
 *     likely that that no other requests arrived from the timer (50ms are quite
 *     a lot), and the only request in IRR is 0x40 which will be forwarded to
 *     the CPU;
 *  8. when the CPU reaches iretq in a_timer_driver, accepts the new request,
 *     jumps to the keyboard driver subroutine and prints the break code; during
 *     the execution time of c_keyboard_driver() the interrupt requests are
 *     again disabled and the timer-spinner is again halted;
 *  9. it is very likely that while printing the make code and the break code
 *     the CPU never goes back to the main program; this is why the spinner on
 *     the left hangs.
 */

