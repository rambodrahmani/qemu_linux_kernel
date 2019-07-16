#*******************************************************************************
# File: idt.s
#       Allocates memory space for the Interrupt Descriptor Table (IDT).
#       The Interrupt Descriptor Table, or IDT, is used in order to show the
#       processor what Interrupt Service Routine (ISR) to call to handle either
#       an exception or an 'int' opcode (in assembly). IDT entries are also
#       called by Interrupt Requests whenever a device has completed a request
#       and needs to be serviced.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.DATA
.balign 16
#-------------------------------------------------------------------------------
.GLOBAL idt
#-------------------------------------------------------------------------------
idt:
    .space 16 * 256, 0
#*******************************************************************************

#*******************************************************************************
# The Interrupt Descriptor Table (theory)
# There are times when you want to interrupt the processor. You want to stop it
# doing what it is doing, and force it to do something different. An example of
# this is when an timer or keyboard interrupt request (IRQ) fires. An interrupt
# is like a POSIX signal - it tells you that something of interest has happened.
# The processor can register 'signal handlers' (interrupt handlers) that deal
# with the interrupt, then return to the code that was running before it fired.
# Interrupts can be fired externally, via IRQs, or internally, via the 'int n'
# instruction. There are very useful reasons for wanting to do fire interrupts
# from software, but that's for another chapter!
#
# The Interrupt Descriptor Table tells the processor where to find handlers for
# each interrupt. It is very similar to the GDT. It is just an array of entries,
# each one corresponding to an interrupt number. There are 256 possible
# interrupt numbers, so 256 must be defined. If an interrupt occurs and there is
# no entry for it (even a NULL entry is fine), the processor will panic and
# reset.
#*******************************************************************************

