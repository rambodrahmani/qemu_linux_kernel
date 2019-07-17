#*******************************************************************************
# File: double_fault.s
#       Double fault.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL double_fault
#-------------------------------------------------------------------------------
double_fault:
    movq  $8, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 8 -- Double Fault
# Normally, when the processor detects an exception while trying to invoke the
# handler for a prior exception, the two exceptions can be handled serially. If,
# however, the processor cannot handle them serially, it signals the
# double-fault exception instead. To determine when two faults are to be
# signalled as a double fault, the 80386 divides the exceptions into three
# classes: benign exceptions, contributory exceptions, and page faults.
#
# Table 9-4 shows which combinations of exceptions cause a double fault and
# which do not.
#
# The processor always pushes an error code onto the stack of the double-fault
# handler; however, the error code is always zero. The faulting instruction may
# not be restarted. If any other exception occurs while attempting to invoke the
# double-fault handler, the processor shuts down.
#
#
# Table 9-3. Double-Fault Detection Classes
#
# Class           ID          Description
#
#                  1          Debug exceptions
#                  2          NMI
#                  3          Breakpoint
#Benign            4          Overflow
#Exceptions        5          Bounds check
#                  6          Invalid opcode
#                  7          Coprocessor not available
#                 16          Coprocessor error
#
#                  0          Divide error
#                  9          Coprocessor Segment Overrun
#Contributory     10          Invalid TSS
#Exceptions       11          Segment not present
#                 12          Stack exception
#                 13          General protection
#
#Page Faults      14          Page fault
#*******************************************************************************

