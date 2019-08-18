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
 * Author: Rambod Rahmani <rambodrahmani@autistici.org>
 *         Created on 17/08/2019.
 */

#include <libqlk.h>
#include <apic.h>

/**
 *
 */
const ioaddr iBR = 0x01F0;

/**
 *
 */
const ioaddr iERR = 0x01F1;

/**
 *
 */
const ioaddr iSCR = 0x01F2;

/**
 *
 */
const ioaddr iSNR = 0x01F3;

/**
 *
 */
const ioaddr iCNL = 0x01F4;

/**
 *
 */
const ioaddr iCNH = 0x01F5;

/**
 *
 */
const ioaddr iHND = 0x01F6;

/**
 *
 */
const ioaddr iCMD = 0x01F7;

/**
 *
 */
const ioaddr iSTS = 0x01F7;

/**
 *
 */
const ioaddr iDCR = 0x03F6;

/**
 *
 */
const ioaddr iASR = 0x03F6;

/**
 * Bus Master IDE Command register (Primary).
 */
ioaddr iBMCMD;

/**
 * Bus Master IDE Status register (Primary).
 */
ioaddr iBMSTR;

/**
 * Bus Master IDE PRD Table Address (Primary).
 */
ioaddr iBMDTPR;

/**
 *
 */
volatile bool done = false;

/**
 *
 */
extern "C" natl prd[];

/**
 *
 */
extern "C" char vv[];

/**
 *
 */
const natl BUFSIZE = 65536;

/**
 *
 */
extern "C" void a_bm_ide();

/**
 *
 */
extern "C" void c_bm_ide()
{
    natb work;

    outputb(0x0A, iDCR);

    inputb(iBMCMD, work);

    work &= 0xFE;

    outputb(work, iBMCMD);

    inputb(iBMSTR, work);

    inputb(iSTS, work);

    done = true;

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
 * Developer harness test shows
 *
 * @param   argc    command line arguments counter.
 * @param   argv    command line arguments.
 *
 * @return          execution exit code.
 */
int main(int argc, char * argv[])
{
    natl base;
    natw cmd;
    natb bus = 0;
    natb dev;
    natb fun;

    // find the bus master IDE controller
    find_bm(dev, fun);

    // the control registers for the controller are allocated via the devices
    // Base Address register at offset 0x20 (32) in PCI configuration space
    base = pci_read_confl(bus, dev, fun, 32);

    base &= 0xFFFFFFFE;

    // set command register address (offset 0 from base address)
    iBMCMD = base;

    // set status register address (offset 2 from base address)
    iBMSTR = base + 2;

    // set 
    iBMDTPR = base + 4;

    cmd = pci_read_confw(bus, dev, fun, 4);

    pci_write_confw(bus, dev, fun, 4, cmd|0x0005);

    gate_init(0x60, a_bm_ide);

    apic_set_VECT(14, 0x60);

    apic_set_MIRQ(14, false);

    natb work;

    natq ww;

    natb nn = 1;

    natl lba = 0;

    for (int i = 0; i < BUFSIZE; i++)
    {
        vv[i] = '-';
    }

    for (int i = 0; i < 80; i++)
    {
        char_write(vv[i]);
    }

    ww = reinterpret_cast<natq>(&vv[0]);

    prd[0] = static_cast<natl>(ww);

    prd[1] = 0x80000000 | ((nn * 512) & 0xFFFF);

    ww = reinterpret_cast<natq>(&prd[0]);

    outputl(static_cast<natl>(ww), iBMDTPR);

    inputb(iBMCMD, work);

    work |= 0x08;

    outputb(work, iBMCMD);

    inputb(iBMSTR,  work);

    work &= 0xF9;

    outputb(work, iBMSTR);

    natb lba_0 = lba,
         lba_1 = lba >> 8,
         lba_2 = lba >> 16,
         lba_3 = lba >> 24;

    outputb(lba_0, iSNR);

    outputb(lba_1, iCNL);

    outputb(lba_2, iCNH);

    natb hnd = (lba_3 & 0x0F) | 0xE0;
    
    outputb(hnd, iHND);
    
    outputb(nn, iSCR);
    
    outputb(0x00, iDCR);
    
    outputb(0xC8, iCMD);
    
    inputb(iBMCMD, work);
    
    work |= 0x01;
    
    outputb(work, iBMCMD);
    
    while (!done) {}
    
    for (int i = 0; i < 80; i++)
    {
        char_write(vv[i]);
    }
    
    pause();

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

