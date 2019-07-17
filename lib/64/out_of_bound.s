#*******************************************************************************
# File: out_of_bound.s
#       Out of bounds exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL out_of_bound
#-------------------------------------------------------------------------------
out_of_bound:
    movq  $5, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 5 -- Bounds Check
# This fault occurs when the processor, while executing a BOUND instruction,
# finds that the operand exceeds the specified limits. A program can use the
# BOUND instruction to check a signed array index against signed limits defined
# in a block of memory.
#*******************************************************************************

