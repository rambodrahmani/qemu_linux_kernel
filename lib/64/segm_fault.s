#*******************************************************************************
# File: segm_fault.s
#       Segment not present exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 16/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL segm_fault
#-------------------------------------------------------------------------------
segm_fault:
    movq  $11, %rdi
    popq  %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 11 -- Segment Not Present
# Exception 11 occurs when the processor detects that the present bit of a
# descriptor is zero. The processor can trigger this fault in any of these
# cases:
#
# - While attempting to load the CS, DS, ES, FS, or GS registers; loading the SS
#   register, however, causes a stack fault.
# - While attempting loading the LDT register with an LLDT instruction; loading
#   the LDT register during a task switch operation, however, causes the
#   "invalid TSS" exception.
# - While attempting to use a gate descriptor that is marked not-present.
#
# This fault is restartable. If the exception handler makes the segment present
# and returns, the interrupted program will resume execution.
#*******************************************************************************

