/**
 * File: pci-ide-bus-mastering.cpp
 *       In this I/O example we will implement a little program capable of
 *       writing to the hard disk drive in bus mastering mode, with reference to
 *       Bus Master IDE Controller (specifications can be found in the reference
 *       PDF file 'Programming Interface for Bus Mastering.pdf'). This document
 *       defines a register level programming interface [0] for a bus master ATA
 *       compatible (IDE) diskcontroller that directly moves data between IDE
 *       devices and main memory. The operations needed are explained in section
 *       3.1.
 *
 *       To make sure the transfer in bus mastering is done correctly, we will
 *       initialize the virtual machine HDD with '@' chars:
 *          perl -e ’print "@"x65536’ |
 *              dd of= ̃HOME/QEMU_LINUX_KERNEL/share/hd.img conv=notrunc
 *
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 17/08/2019.
 */

#include <libqlk.h>
#include <apic.h>
#include <pci.h>
#include <hdd.h>

/**
 * Bus Master IDE Command register (Primary).
 */
ioaddr iBMCMD;

/**
 * Bus Master IDE Status register (Primary).
 */
ioaddr iBMSTR;

/**
 * Bus Master IDE Descriptor Table Pointer register (Primary).
 */
ioaddr iBMDTPR;

/**
 * Set to true when the bus master transfer is done.
 */
volatile bool done = false;

/**
 *
 */
extern "C" natl prd[];

/**
 * Memory buffer where to store the data retrieved from the HDD.
 */
extern "C" char vv[];

/**
 * Memory buffer size.
 */
const natl BUFSIZE = 65536;

/**
 * Primitive 60 Assembly implementation.
 */
extern "C" void a_bm_ide();

/**
 * Primitive 60 C++ body implementation.
 */
extern "C" void c_bm_ide()
{
    natb work;

    outputb(0x0A, iDCR);

    // clear our the start/stop bit in BMCD
    inputb(iBMCMD, work);
    work &= 0xFE;
    outputb(work, iBMCMD);

    // read the bm and the ATA controller status registers content: interrupt
    // request handled
    inputb(iBMSTR, work);
    inputb(iSTS, work);

    // set to true: the bus master transfer is done
    done = true;

    // send end of interrupt
    apic_send_EOI();
}

/**
 * Finds the bus master IDE controller and leaves the device and the
 * functionality number in dev and fun.
 * For simplicity we will search only on bus 0. From the specs we now that the
 * bus master Class Code in PCI configuration space indicates IDE device (top
 * two bytes have the value 0x0101).
 *
 * @param   dev
 * @param   fun
 */
void find_bm(natb& dev, natb& fun)
{
    for (dev = 0; dev < 32; dev++)
    {
        for (fun = 0; fun < 8; fun++)
        {
            if (pci_read_confw(0, dev, fun, 10) == 0x0101)
            {
                return;
            }
        }
    }
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
    // base register address
    natl base;

    // command register content
    natw cmd;

    // bus (0), device and function of the bus master
    natb bus = 0;
    natb dev;
    natb fun;

    // find the bus master IDE controller
    find_bm(dev, fun);

    // the control registers for the controller are allocated via the devices
    // Base Address register at offset 0x20 (32) in PCI configuration space
    base = pci_read_confl(bus, dev, fun, 32);

    // set bit n. 0 to 0
    base &= 0xFFFFFFFE;

    // set command register address (offset 0 from base address)
    iBMCMD = base;

    // set status register address (offset 2 from base address)
    iBMSTR = base + 2;

    // set descriptor table pointer register address (offset 2 from base
    // address)
    iBMDTPR = base + 4;

    cmd = pci_read_confw(bus, dev, fun, 4);

    pci_write_confw(bus, dev, fun, 4, cmd | 0x0005);

    // initialize IDT entry 60 with the a_bm_ide address
    gate_init(0x60, a_bm_ide);

    // set interrupt 60 for the APIC pin 14
    apic_set_VECT(14, 0x60);

    // enable APIC pin 14 interrupt requests
    apic_set_MIRQ(14, false);

    natb work;

    natq ww;

    // number of sectors to be read
    natb sectors = 1;

    // first sector to read
    natl lba = 0;

    // fill in the memory buffer with '-' chars to make sure that the memory
    // buffer will be filled (with different chars) by the bus master transfer
    for (int i = 0; i < BUFSIZE; i++)
    {
        vv[i] = '-';
    }

    for (int i = 0; i < 80; i++)
    {
        char_write(vv[i]);
    }

    // PCI bus master initialization (ATA-PCI bridge)
    ww = reinterpret_cast<natq>(&vv[0]);

    // prepare descriptor table
    prd[0] = static_cast<natl>(ww);
    prd[1] = 0x80000000 | ((sectors * 512) & 0xFFFF);

    // descriptor table pointer
    ww = reinterpret_cast<natq>(&prd[0]);

    // write descriptor table pointer
    outputl(static_cast<natl>(ww), iBMDTPR);

    // read command register content
    inputb(iBMCMD, work);

    // bit n. 2 set to 0 (PCI write transfer)
    work |= 0x08;

    // write new value to the command register
    outputb(work, iBMCMD);

    // read status register
    inputb(iBMSTR,  work);

    // clear bit n. 1-2 (errors and interrupt requests falgs)
    work &= 0xF9;

    // write value to the status register
    outputb(work, iBMSTR);

    // ATA controller initialization
    natb lba_0 = lba,
         lba_1 = lba >> 8,
         lba_2 = lba >> 16,
         lba_3 = lba >> 24;

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

    // enable interrupt requests
    outputb(0x00, iDCR);

    // direct access mode read command
    outputb(0xC8, iCMD);

    // read bm command register
    inputb(iBMCMD, work);

    // set bit n. 1 to 1 (start/stop)
    work |= 0x01;

    // write new value to the command register: start PCI transfer
    outputb(work, iBMCMD);

    // wait for the done flag to be set
    while (!done) {}

    // write the retrieved data to the video output
    for (int i = 0; i < 80; i++)
    {
        char_write(vv[i]);
    }

    // print message and wait for the ESC key
    pause();

    // return with no errors
    return 0;
}

/**
 * [0]
 * The programming interface defines a simple scatter/gather mechanism allowing
 * large transfer blocks to bescattered to or gathered from memory. This cuts
 * down on the number of interrupts to and interactions withthe CPU.
 * In computing, vectored I/O, also known as scatter/gather I/O, is a method of
 * input and output by which a single procedure call sequentially reads data
 * from multiple buffers and writes it to a single data stream, or reads data
 * from a data stream and writes it to multiple buffers, as defined in a vector
 * of buffers. Scatter/gather refers to the process of gathering data from, or
 * scattering data into, the given set of buffers. Vectored I/O can operate
 * synchronously or asynchronously. The main reasons for using vectored I/O are
 * efficiency and convenience.
 */

