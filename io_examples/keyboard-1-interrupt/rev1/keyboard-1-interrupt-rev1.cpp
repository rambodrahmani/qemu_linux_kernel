/**
 * File: keyboard-1-interrupt-rev1.cpp
 *       Revision 1.
 *       In this second attempt, we introduce the a_driver() method implemented
 *       in a_driver.a which calls the c_driver() method and returns using
 *       iretq. This is not the onyl fix we need. Actually, we will get the
 *       exact same result unless we introduce the apic_send_EOI() utility
 *       method which sends the End Of Interrupt signal. To be precise the
 *       library initializes pin 1 as IO-APIC-level. This means that the APIC
 *       will recognize an interrupt request whenever the the signal changes
 *       from 0 to 1, and won't send further interrupt requests coming on that
 *       pin unless the End Of Interrupt is received.
 *
 *       Compile using:
 *          ./compile
 *
 *       Execute using:
 *          ./boot
 *
 *       Output:
 *          This implementation looks like to be working just fine however there
 *          are several mistakes. We can group these mistakes into 2 main
 *          groups:
 *              1. those related to parallel execution which might result in the
 *                 value of the keycode (c) changing before it's printed (one
 *                 keyocde will be missed) or, even worse, while being printed;
 *              2. those related to the compiler.
 *          Revision 2 and 3 will focus on these two problems and fix them.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 06/07/2019.
 */

#include <libqlk.h>
#include <kbd.h>
#include <apic.h>

/**
 * True when a new keycode is retrived from the keyboard.
 */
bool new_char;

/**
 * Keycode retrived from the keyboard.
 */
natb c;

/**
 * 
 */
extern "C" void a_driver();

/**
 * Keyboard driver.
 */
extern "C" void c_driver()
{
    // read a byte from the RBR register to c
    inputb(iRBR, c);

    // set new_char to true
    new_char = true;

    // send EOI
    apic_send_EOI();
}

/**
 * Developer harness test shows how to retrieve keycodes from the keyboard using
 * the interrupt mechanims. [1]
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    // initialize IDT entry 40 with the a_driver address, [2]
    gate_init(0x40, a_driver);

    // set interrupt type for APIC pin number 1
    apic_set_VECT(1, 0x40);

    // enable APIC pin number 1 to send interrupt requests, [3]
    apic_set_MIRQ(1, false);

    // initialize the keyboard with interrupt requests enabled, [0]
    outputb(0x60, iCMR);
    outputb(0x61, iTBR);

    for(;;)
    {
        new_char = false;

        // wait for a new keycode to be retrived, [4]
        while (!new_char) {}

        // check for the ESC key
        if (c == 0x01)
        {
            break;
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
    }

    return 0;
}

/**
 * [0]
 * The interrupt requests can be disabled writing 0x60 in both the CMR and TBR
 * registers. Once an interrupt request is sent, the APIC controller won't
 * present a new one until the content of the RBR register is read.
 * Starting from this point in our program, the keyboard can and will send
 * interrupt requests whenever a key is pressed. This also means that the
 * content of the a_driver method will be executed. From this point on we have
 * to think the execution of our program as separated into 2 main execution
 * flows: the one of the main program and the one running the interrupt
 * subroutine. This secondary execution flow can kick in parallely to complete
 * its task. The randomness of the execution of the subroutine forces us to use
 * global variables in order to be able to exchange data (variables c and
 * new_char).
 */

/**
 * [1]
 * The idea behin this very first implementation is to make so that the a_driver
 * function is called everytime the keyboard controller sends an interrupt
 * requests. The keyboard is connected to APIC pin number 1. We first choose an
 * empty entry in the IDT (keep in mind that the first 32 entries are
 * system-reserved). We will be using entry number 40 (0x40) which will also be
 * the type for the interrupt requests coming from the keyboard. This means that
 * everytime the APIC receives an interrupt requests on pin number 1, it will
 * forward to the CPU an interrupt request having type 40 (the CPU will take
 * care of retriving the subroutine - driver - to be executed from the IDT
 * etc...).
 */

/**
 * [2]
 * Keep in mind that the gate created using gate_init() will have the TI flag
 * set to 0. Therefore the processor will reset IF in the RFLAGS register before
 * jumping to the subroutine).
 */

/**
 * [3]
 * The APIC can manage interrupt requests pins independently. When the program
 * is loaded, libqlk will disable all APIC pins. We can then enable the ones we
 * need.
 */

/**
 * [4]
 * Whenever new_char is set to True means that a_driver. This also means that
 * the keyboard sent an interrupt request and that a new key was pressed.
 * a_driver will retrive the keycode contained in the RBR register of the
 * keyboard controller.
 *
 * In this example you can perfectly see that we are not taking advantage of the
 * interrupt mechanism by carrying on other tasks.
 */

