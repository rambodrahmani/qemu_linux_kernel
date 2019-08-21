/**
 * File: init_keyboard.cpp
 *       Initializes the IDT, the APIC pins and the keyboard with interrupt
 *       requests enabled.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>
#include <apic.h>
#include <kbd.h>

/**
 * Use semaphore number 1 for synchronization.
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
 * Interrupt 241 primitive.
 */
extern "C" void a_read_line();

/**
 * Interrupt 41 primitive.
 */
extern "C" void a_keyboard_driver();

/**
 * Initializes custom IDT entries and the APIC pins.
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
 *
 * Sets the chars counter value and the destination memory buffer address.
 * Initializes the keyboard with interrupt requests enabled and waits for
 * c_keyboard_driver() to signal the end of chars transfer.
 */
extern "C" void c_read_line(int& len, char buf[])
{
    int old_len = len;

    // set chars counter value
    counter = len;

    // set destination memory buffer address
    pointer = buf;

    // enable keyboard interrupt requests
    outputb(0x60, iCMR);
    outputb(0x61, iTBR);

    // wait for c_driver_keyboard() to finish retrieving chars
    sem_wait(sincr_t);

    // return number of retrived chars
    len = old_len - counter;
}

/**
 * New implementation for the get_code() function to work with the interrupt
 * mechanism. There is no need to check for the status register to wait for a
 * new ket to be pressed.
 */
natb get_code_int()
{
    natb c;

    inputb(iRBR, c);

    return c;
}

/**
 * C++ implementation for the a_keyboard_drive subroutine.
 * This method is called whenever the keyboard send an interrupt request.
 *
 * Disables the keyboard interrupt requests. Retrieves the keycode from the
 * keyboard, gets the corresponding ASCII char. If a valid char is not retrieved
 * (c == 0), keyboard interrupt are enabled and the method returns. Otherwise,
 * the chars counter is decremented, the ASCII char is inserted into the memory
 * buffer and printed to the video output.
 *
 * Before enabling keyboard interrupt again and returning, it checks if the new
 * line char was retrieved or the maximum number of chars was reached.
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

    // get ASCII char corresponding to the keycode
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

    // check if new line was received or maximum number of chars was reached
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

