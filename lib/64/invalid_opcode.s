#*******************************************************************************
# File: invalid_opcode.s
#       Invalid opcode exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL invalid_opcode
#-------------------------------------------------------------------------------
invalid_opcode:
    movq  $6, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 6 -- Invalid Opcode
# This fault occurs when an invalid opcode is detected by the execution unit.
# (The exception is not detected until an attempt is made to execute the invalid
# opcode: prefetching an invalid opcode does not cause this exception.)
#
# This exception also occurs when the type of operand is invalid for the given
# opcode. Examples include an intersegment JMP referencing a register operand,
# or an LES instruction with a register source operand. 
#*******************************************************************************

