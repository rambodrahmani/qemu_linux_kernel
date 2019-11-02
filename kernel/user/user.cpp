#line 1 "user/prog/primitive.in"
/**
 * File: primitive.in
 *       This user program example shows how to add a new primitive to the
 *       kernel for the User Module. Inside the System and I/O Modules all the
 *       source codes related to this example will be marked using the
 *       USER-PRIMITIVE-EXAMPLE tag.
 *
 *       For this example we will add the getid() primitive which can be used by
 *       the User Module to retrieve the id of the process where it's called. It
 *       does not take any actual arguments and just return the calling process
 *       id. In order to achieve our objective, the following implementations
 *       must be done in the Kernel:
 *        1. Assign an interrupt type to the primitive. All interrupt types are
 *           defined in kernel/include/constants.h. Keep in mind that all 
 *           interrupt types are defined as C++ macros in order to be able to
 *           use them in both C++ and Assembly code;
 *        2. Initialize an IDT gate for this interrupt. We will have to call the
 *           load_gate macro in the init_idt function defined in system/system.s
 *           specifying the interrupt type defined in kernel/include/constants.h
 *           the assembly subroutine to be called (a_getid) abd the DPL
 *           (LEV_USER);
 *        3. Define the a_getid assembly subroutine in system/system.s which
 *           will only call the corresponding C++ implementation c_getid();
 *        4. Write C++ implementation c_get() in system/system.cpp;
 *        5. Point 1-4 contain all the modifications needed for the System
 *           module. We can now compile the system module using make to make
 *           sure everything was implemented correctly;
 *        6. We must now provide to the User module a way to call the c_getid()
 *           function (use interrupt TIPO_GETID). To do so, we declare the
 *           getid() interface program in sys.h;
 *        7. And define the getid() interface program in user.s.
 *        8. Everything is ready now for the User Module to be able to use the
 *           getid() as shown in the example below.
 *
 *       After placing the file in user/prog/primitive.in compile und run using:
 *          make
 *          make swap
 *          ./run
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 07/09/2019
 */

#include <sys.h>
#include <lib.h>

/**
 * User Level process implemented by sample_body having priority 20.
 */

#line 52 "user/prog/primitive.in"
/**
 * Each process_body definition corresponds to a void function having a single
 * integer parameter. This is done for simplicity. The integer parameter can be
 * omitted in the definition.
 *
 * Developer harness test shows how to use the getid() primitive.
 *
 * @param  a  process void function argument.
 */
void sample_body(int a)
#line 62 "user/prog/primitive.in"
{
    // char buffer
    char buf[10];

    // retrieved process id
    natl id;

    // write message to the video output
    writeconsole("My process id: ");

    // user the getid() primitive to retrieve process id
    id = getid();

    // convert id into string and write it in the char buffer
    int_conv(id, buf);

    // write the content of buf to the video output
    writeconsole(buf);

    // print pause message and wait for the ESC key
    pause();

	terminate_p();
}
short sample;
#line 90 "user/user.cpp"

int main()
{
	sample = activate_p(sample_body, 0, 20, LEV_USER);

	terminate_p();
}
