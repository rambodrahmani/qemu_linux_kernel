/**
 * File: break-point.cpp
 *       In order to be able to point out the differences between handling a
 *       fault exception and a trap one, in this sample user program we will
 *       invoke a break-point exception (trap) type number 3. This exception
 *       can be ivoked using the assembly instruction int3.
 *       Because the CPU is handling a trap exception, it must save in the stack
 *       the address of the next instruction. After compiling this user program,
 *       use the command:
 *          objdump -d | grep -C 1 utente.cpp
 *       which will provide an output similar to
 *
 *       After copying this file into user/user.cpp compile and run using
 *          make
 *          make swap
 *          ./run
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 31/08/2019.
 */

/**
 * Developer harness test.
 *
 * @param  argc  command line arguments counter.
 * @param  argv  command line arguments.
 *
 * @return       execution exit code.
 */
int main(int argc, char * argv[])
{
    int x = 0;

    x++;

    // invoke assembly int3 instruction
    asm("int3");

    x++;

    return 0;
}

