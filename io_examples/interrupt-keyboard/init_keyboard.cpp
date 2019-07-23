/**
 * File: init_keyboard.cpp
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>
#include <apic.h>
#include <kbd.h>

/**
 * Use semaphore number 1.
 */
const natl sincr_t = 1;

/**
 *
 */
natl counter;

/**
 *
 */
char* pointer;

/**
 *
 */
extern "C" void a_read_line();

/**
 *
 */
extern "C" void a_keyboard_driver();

/**
 * Initializes the IDT and the APIC pins.
 */
void ini()
{
    // set interrupt 241 for the primitive
    gate_init(241, a_read_line);

    // set interrupt 41 for the driver
    gate_init(41, a_keyboard_driver);

    // set interrupt 41 for APIC pin 1
    apic_set_VECT(1, 41);

    // enable APIC pin number 1
    apic_set_MIRQ(1, false);
}

/**
 * C++ implementation for the a_read_line primitive.
 */
extern "C" void c_read_line(int& len, char buf[])
{
    int old_len = len;
    
    counter = len;

    pointer = buf;

    // enable keyboard interrupt requests
    outputb(0x60, iCMR);
    outputb(0x61, iTBR);

    sem_wait(sincr_t);

    len = old_len - counter;
}

/**
 *
 */
natb get_code_int()
{
    natb c;

    inputb(iRBR, c);

    return c;
}

/**
 * C++ implementation for the a_keyboard_drive subroutine.
 */
extern "C" void c_keyboard_driver()
{
    // retrieved keycode from the keyboard
    natb b;

    // corresponding ASCII char
    char c;

    // disable keyboard interrupt requests
    outputb(0x60, iCMR);
    outputb(0x60, iTBR);

    // retrieve keycode from the keyboard
    b = get_code_int();

    if (b == 0x2A)          // left shift make code
    {
        shift = true;
    }
    else if (b == 0xAA)     // left shift break code
    {
        shift = false;
    }

    // get ASCII code corresponding to the keycode
    c = keycode_to_ascii(b);

    if (c == 0)
    {
        // enable keyboard interrupt requests again
        outputb(0x60, iCMR);
        outputb(0x61, iTBR);

        return;
	}

    // decrese chars counter
	counter--;

    // insert ASCII char in the buffer
	*pointer = c;

    // write ASCII char to the video output
	char_write(c);

    // check if new line was received or maximum number of chars was received
	if ((c == '\n') || (counter == 0))
    {
        // signal last char retrieved
        sem_signal(sincr_t);
        
        return;
	}

    // increase ASCII chars buffer pointer
	pointer++;

    // enable keyboard interrupt requests
	outputb(0x60, iCMR);
	outputb(0x61, iTBR);
}

