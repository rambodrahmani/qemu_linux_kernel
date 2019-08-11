/**
 * File: keyboard-1-interrupt-rev3.cpp
 *       Revision 3.
 *       The second type of errors which our initial implementation is affected
 *       by are related to compiler optimizations. By default, compiler
 *       optimizations are disabled in the provided compile script. To enable
 *       them we can use the -On option where 1 <= n <= 3. For example, we can
 *       edit the compile script and add '-O2' to COMPILER_OPTIONS in
 *       $HOME/QEMU_LINUX_KERNEL/bin/compile. If we now try to compile and run
 *       our program, nothing will be printed to the video output.
 *
 *       The problem arises because the C++ compiler does not know about the
 *       interrupt requests that might arrive at any given time. And presumes
 *       that no external process can interfere with the execution of a
 *       function. Therefore it will presume that nothing can change the value
 *       of the variable new_char at line 102 and consequently transform the
 *       while loop into an if-condition like this:
 *
 *          if (!new_char)
 *              while (true);
 *
 *       which will result in an infinite loop.
 *       In order to fix this we have to let the compiler know that both the
 *       new_char and c variables can be changed unexpectedly. C++ allows us to
 *       do so by declaring the variables volatile.
 *
 *       Compile using:
 *          ./compile
 *
 *       Execute using:
 *          ./boot
 *
 *       Output:
 *         Everything works fine now and there no other hidden mostakes that
 *         might arise as far as it concerns the C++ side of the implementation.
 *         However, there is one last point we have to consider regarding the
 *         usage of the registers made by the method c_driver().
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
volatile bool new_char;

/**
 * Keycode retrived from the keyboard.
 */
volatile natb c;

/**
 * 
 */
extern "C" void a_driver();

/**
 * Keyboard driver.
 */
extern "C" void c_driver()
{
    // temp variable, can't pass a volatile variable as reference
    natb cc;

    // diable keyboard interrupt requests before reading RBR
    // [comment this out to see the parallel execution bugs]
    outputb(0x60, iCMR);
    outputb(0x60, iTBR);

    // read a byte from the RBR register to c
    inputb(iRBR, cc);

    // assign non volatile to volatile variable
    c = cc;

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

