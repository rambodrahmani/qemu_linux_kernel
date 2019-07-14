/**
 * File: timer-1.cpp
 *       The output of PIT channel 2 is connected to the PC speaker, so the
 *       frequency of the output determines the frequency of the sound produced
 *       by the speaker. This is the only channel where the gate input can be
 *       controlled by software (via bit 0 of I/O port 0x61), and the only
 *       channel where its output (a high or low voltage) can be read by
 *       software (via bit 5 of I/O port 0x61).
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 14/07/2019.
 */

#include <libqlk.h>

/**
 * Speaker Register I/O address.
 */
const ioaddr iSPR = 0x61;

/**
 * Mode/Command register I/O port address.
 */
const ioaddr iCWR = 0x43;

/**
 * Channel 2 data I/O port address (LSB).
 */
const ioaddr iCTR2_LSB = 0x42;

/**
 * Channel 2 data I/O port address (LSB).
 */
const ioaddr iCTR2_MSB = 0x42;

/**
 * Initializes channel 2 in mode 3.
 */
void ini_cont()
{
    // time constant for 1000 Hz
    natw N = 1190;

    // set mode 3 for channel 2
    outputb(0xB6, iCWR);

    // time constant LSB
    outputb(N, iCTR2_LSB);

    // time constant MSB
    outputb(N >> 8,  iCTR2_MSB);
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
    // initialize channel 2
    ini_cont();
    
    outputb(3, iSPR);
    
    // wait for esc key
    pause();
    
    outputb(0, iSPR);
    
    // return
    return 0;
}

