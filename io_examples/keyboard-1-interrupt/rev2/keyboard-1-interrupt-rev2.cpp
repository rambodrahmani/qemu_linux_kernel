/**
 * File: keyboard-1-interrupt-rev2.cpp
 *       Revision 2.
 *       The third implementation for our example will take of the problems
 *       introduced by parallel executiion. As we previously said, it might
 *       happen that the keycode changes (multiple interrupt requests sent too
 *       fast) before we can print it or, even worse, while we are printing it.
 *       To be able to showcase this problems, we add a for loop which will slow
 *       down the printing of the keycode. With this edit, even pressing the
 *       same key we can get different keycodes.
 *
 *       To fix such problems, we disable keyboard interrupt requests before
 *       reading from the RBR register, and enable them back after setting
 *       new_char to false.
 *
 *       Compile using:
 *          ./compile
 *
 *       Execute using:
 *          ./boot
 *
 *       Output:
 *          With this fixes, it looks like again as if the program works just
 *          fine. However, there still some mistakes. These mistakes will show
 *          up only if we enable compiler optimizations (which are disabled by
 *          default in the compile script). Revision 3 will take care of showing
 *          this mistakes and fixing them.
 *          
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 11/08/2019.
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
    // diable keyboard interrupt requests before reading RBR
    // [comment this out to see the parallel execution bugs]
    outputb(0x60, iCMR);
    outputb(0x60, iTBR);

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

        // enbale keyboard interrupt requests again
        // [comment this out to see the parallel execution bugs]
        outputb(0x60, iCMR);
        outputb(0x61, iTBR);

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

            // slow down
            for (int j = 0; j < 1000000; j++) {}
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

