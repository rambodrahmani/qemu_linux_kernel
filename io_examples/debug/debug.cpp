/**
 * File: debug.cpp
 *       In this I/O example we will see how to work with the CPU in single-step
 *       mode. A trap flag permits operation of a processor in single-step mode.
 *       If such a flag is available, debuggers can use it to step through the
 *       execution of a computer program.
 *       When a system is instructed to single-step, it will execute one
 *       instruction and then stop. The contents of registers and memory
 *       locations can be examined; if they are correct, the system can be told
 *       to go on and execute the next instruction. The Intel 8086 trap flag and
 *       type-1 interrupt response make it quite easy to implement a single-step
 *       feature in an 8086-based system. If the trap flag is set, the 8086 will
 *       automatically do a type-1 interrupt after each instruction executes.
 *       When the 8086 does a type-1 interrupt, it pushes the flag register on
 *       the stack.
 *
 *       After copying this file into user/user.cpp compile and run using
 *          make
 *          make swap
 *          ./run
 *
 *       Output:
 *       The log messages sent using the flog function will be sent to the
 *       serial interface COM1 which is conneted to the std output of the
 *       console you use to run the virtual machine. Pay attention to that
 *       console to see these messages.
 *
 *          [...]
 *          INF	0	entry point 00200120
 *          INF	0	bochsvga: framebuffer initialized on 00000000fd000000 and vbext on 00000000febf0500
 *          DBG	0	rip = 0000000000200197
 *          DBG	0	rip = 000000000020019b
 *          DBG	0	rip = 000000000020019f
 *          DBG	0	rip = 00000000002001a3
 *          DBG	0	rip = 00000000002001a7
 *          DBG	0	rip = 00000000002001c1
 *          DBG	0	rip = 00000000002001c2
 *          DBG	0	rip = 00000000002001c8
 *          DBG	0	rip = 00000000002001c9
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 31/08/2019.
 */

#include <libqlk.h>
#include <log.h>

/**
 *
 */
extern "C" void enable_single_step();

/**
 *
 */
extern "C" void disable_single_step();

/**
 *
 */
extern "C" void a_debug();

/**
 *
 */
extern "C" void c_debug(void * rip)
{
    flog(LOG_DEBUG, "rip = %p", rip);
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
    int x = 0;

    gate_init(1, a_debug);

    enable_single_step();

    x++;
    x++;
    x++;
    x++;

    disable_single_step();

    pause();

    return 0;
}

