/**
 * File: mod_rdma.cpp
 *       The PCI-ATA bridge implements the BMCMD (8 bits + reserved byte)
 *       BMSTR (8 bits + reserved byte) and BMDTPR (32 bits) registers fot
 *       both ATA-0 and ATA-1 channels in a single 16 bytes long block in the
 *       I/O space. Once the bridge is found on the bus using its unique Class
 *       Code, it is possible to access these register using the correspoding
 *       offset relative to the base register and configure the Bus Mastering
 *       mode.
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 28/08/2019.
 */

#include <libqlk.h>
#include <hdd.h>
#include <pci.h>
#include <apic.h>

/**
 * The bus functions with bus mastering capabilities must implement at least 3
 * register in either the memory or I/O space. These registers have an address
 * (offset) relative to the Base Address of the function. [0]
 */

/**
 * Bus Master IDE Command Register.
 * Refer to 'Programming Interface for Bus Mastering.pdf' for specs.
 */
ioaddr iBMCMD;

/**
 * Bus Master IDE Status Register.
 * Refer to 'Programming Interface for Bus Mastering.pdf' for specs.
 */
ioaddr iBMSTR;

/**
 * Descriptor Table Pointer Register.
 * Refer to 'Programming Interface for Bus Mastering.pdf' for specs.
 */
ioaddr iBMDTPR;

/**
 *
 */
const natl sincr = 11;

/**
 *
 */
natl prd[2];

/**
 *
 */
extern "C" void a_go_wdma();

/**
 *
 */
extern "C" void a_driver_wdma();

/**
 * Finds the bus master IDE controller and leaves the device and the
 * functionality number in dev and fun.
 * For simplicity we will search only on bus 0. From the specs we now that the
 * bus master Class Code in the PCI configuration space indicates IDE device
 * (top two bytes have the value 0x0101).
 *
 * @param  dev  bus master device number;
 * @param  fun  bus master function number.
 */
void find_bm(natb& dev, natb& fun)
{
    // loop through all device on bus 0
    for (dev = 0; dev < 32; dev++)
    {
        // loop through all functions for each device
        for (fun = 0; fun < 8; fun++)
        {
            // check the class code for each function
            if (pci_read_confw(0, dev, fun, 10) == 0x0101)
            {
                // PCI-ATA bridge found
                return;
            }
        }
    }
}

/**
 * Finds the IDE bus master on bus 0 and initializes it to work in bus mastering
 * mode.
 */
void init_bm()
{
    // base address
    natl base;

    // command register
    natw cmd;

    // bus master bus (0), device and function
    natb bus = 0, dev, fun;

    // find the IDE bus master on bus 0
    find_bm(dev, fun);

    // read the base register content
    base = pci_read_confl(bus, dev, fun, 32);

    // set bit n. 0 to 0
    base &= 0xFFFFFFFE;

    // set BM command register address
    iBMCMD = base;

    // set BM status register address
    iBMSTR = base + 2;

    // set BM table pointer register
    iBMDTPR = base + 4;

    // read BM command register content
    cmd = pci_read_confw(bus, dev, fun, 4);

    // enable bus mastering (bin n. 0 and n. 2 set to 1)
    pci_write_confw(bus, dev, fun, 4, cmd | 0x0005);
}

/**
 * Initializes custom IDT entries and the corresponding APIC pins.
 */
void ini()
{
    // find and initialze the IDE bus master
    init_bm();

    // initialize IDT entry 251 with a_go_wdma address
	gate_init(251, &a_go_wdma);

    // initialize IDT entry 51 with a_driver_wdma address
	gate_init(51, &a_driver_wdma);

    // set interrupt 51 for APIC pin 14
	apic_set_VECT(14, 51);

    // enable APIC pin 14 interrupt requests
	apic_set_MIRQ(14, false);
}

/**
 *
 */
extern "C" void c_go_wdma(natl indse, natb sectors, natb vv[])
{
    natb work;
    natq ww;

    // PCI bus master initialization (ATA-PCI bridge)
    ww = reinterpret_cast<natq>(&vv[0]);

    // prepare descriptor table
    prd[0] = static_cast<natl>(ww);
    prd[1] = 0x80000000 | sectors*512;

    // descriptor table pointer
    ww = reinterpret_cast<natq>(&prd[0]);

    // write descriptor table pointer
    outputl(static_cast<natl>(ww), iBMDTPR); 

    // read command register content
    inputb(iBMCMD, work);

    // bit n. 2 set to 0 (PCI write transfer)
    work &= 0xF7; 

    // write new value to the command register
    outputb(work, iBMCMD);

    // read status register
    inputb(iBMSTR, work);

    // clear bit n. 1-2 (errors and interrupt requests falgs)
    work &= 0xF9;

    // write value to the status register
    outputb(work, iBMSTR);

    // ATA controller initialization
    natb lba_0 = indse,
         lba_1 = indse >> 8,
         lba_2 = indse >> 16,
         lba_3 = indse >> 24;

    // set start sector address
    outputb(lba_0, iSNR);

    // set cylinder number (low)
    outputb(lba_1, iCNL);

    // set cylinder number (high)
    outputb(lba_2, iCNH);

    // select hard disk drive number
    natb hnd = (lba_3 & 0x0F) | 0xE0;
    outputb(hnd, iHND); 

    // set sectors number
    outputb(sectors, iSCR);

    // enable ATA controller interrupt requests
    outputb(0x00, iDCR);

    // direct access mode write command
    outputb(0xCA, iCMD);

    // read bm command register
    inputb(iBMCMD, work);

    // set bit n. 1 to 1 (start/stop)
    work |= 0x01;

    // write new value to the bm command register: start PCI transfer
    outputb(work, iBMCMD);

    // wait for semaphore sincr
    sem_wait(sincr);
}

/**
 *
 */
extern "C" void c_driver_wdma()
{
    natb work;

    inputb(iBMCMD, work);

    work &= 0xFE;

    outputb(work, iBMCMD);

    inputb(iBMSTR, work);

    inputb(iSTS, work);

    outputb(0x02,iDCR); 

    sem_signal(sincr);
}

/**
 * [0]
 * 2.0. Bus Master IDE Register Description
 * The bus master IDE function uses 16 bytes of IO space. All bus master IDE IO
 * space registers can beaccessed as byte, word, or Dword quantities. The
 * description of the 16 bytes of IO registers follows:
 *
 * Offset from     Register                                      Register Access
 * Base Address
 * -----------------------------------------------------------------------------
 * 00h             Bus Master IDE Command register Primary       R/W
 * 01h             Device Specific
 * 02h             Bus Master IDE Status register Primary        R/W/C
 * 03h             Device Specific
 * 04h-07h         Bus Master IDE PRD Table Address Primary      R/W
 * 08h             Bus Master IDE Command register Secondary     R/W
 * 09h             Device Specific
 * 0Ah             Bus Master IDE Status register Secondary      R/W/C
 * 0Bh             Device Specific
 * 0Ch-0Fh         Bus Master IDE PRD Table Address Secondary    R/W
 */

