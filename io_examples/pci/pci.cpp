/**
 * File: pci.cpp
 *       This I/O example shows how to use the library utility functions to
 *       prints all the functions available for all the devices on the PCI bus
 *       0.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 19/07/2019.
 */

#include <libqlk.h>
#include <pci.h>

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
    // we will be working only with bus 0
    natb bus = 0;
    natb c;

    // loop through the 32 devices on bus 0
    for (natb dev = 0; dev < 32; dev++)
    {
        // loop through the 8 functions of each device
        for (natb fun = 0; fun < 8; fun++)
        {
            // vendor id for the function
            natw vendorID;

            // device id for the function
            natw deviceID;

            // retrieve vendor id
			vendorID = pci_read_confw(bus, dev, fun, 0);

            // check if the vendor id is valid
			if (vendorID == 0xFFFF)
            {
                continue;
            }

            // retrieve device id
            deviceID = pci_read_confw(bus, dev, fun, 2);

            char buf[20];
            char *ptr = &buf[0];

            // creates a final string to be printed to the video output
            // from the retrieved data

            // convert the bus number to string and append it
            ptr = natb_conv(bus, ptr);

            // append a dot
            ptr = copy(".", ptr);

            // convert the device number to string and append it
            ptr = natb_conv(dev, ptr);

            // append a dot
            ptr = copy(".", ptr);

            // convert the function number to string and append it
            ptr = natb_conv(fun, ptr);

            // append a space
            ptr = copy(" ", ptr);

            // convert the vendor id to string and append it
            ptr = natw_conv(vendorID, ptr);

            // appena a space
            ptr = copy(" ", ptr);

            // convert the device id to string and append it
            ptr = natw_conv(deviceID, ptr);

            // add a new line to the string
            copy("\n", ptr);

            // print the final string to the video output
            str_write(buf);
        }
    }

    // print message on the video output and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

