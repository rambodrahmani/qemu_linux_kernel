/**
 * File: debugger.cpp
 *       In this I/O example we will see how to implement the basic concept
 *       behind a debugger: inserting a break point at a given point in the
 *       program. Keep in mind that we need to use the int3 assembly instruction
 *       which is equal to 0xCC and occupies only one byte.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 05/09/2019.
 */

#include <libqlk.h>
#include <log.h>

/**
 * Breakpoint will be inserted right before the foo() function.
 */
void foo()
{
    // just print a text message to the console
    printf("foo(): have a look to the console for the breakpoint!\n");
}

/**
 *
 */
natb old;

/**
 * Assembly interrupt 3 primitive.
 */
extern "C" void a_debug();

/**
 * Primitive a_debug() C++ body implementation.
 *
 * @param  rip  pointer to %rip.
 */
extern "C" void c_debug(void *rip)
{
    // retrieve address pointer by %rip (next instruction)
    natb * p = reinterpret_cast<natb*>(rip);

    // decrement address
    p--;

    // print log message
    flog(LOG_DEBUG, "Breakpoint at %p reached.", p);

    // replace 
    *p = old;
}

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
    // initialize IDT gate 3 with a_debug address
    gate_init(3, a_debug);

    // retrieve foo function first byte
    natb * p = reinterpret_cast<natb*>(foo);

    // save the first byte in old
    old = *p;

    // replace the first byte of the foo function with int3
    *p = 0xCC;

    // call foo function
    foo();

    // print message to the video output and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

