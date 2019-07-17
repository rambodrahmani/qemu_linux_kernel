#*******************************************************************************
# File: overflow.s
#       Overflow exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 16/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL overflow
#-------------------------------------------------------------------------------
overflow:
    movq  $4, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 4 -- Overflow
# This trap occurs when the processor encounters an INTO instruction and the OF
# (overflow) flag is set. Since signed arithmetic and unsigned arithmetic both
# use the same arithmetic instructions, the processor cannot determine which is
# intended and therefore does not cause overflow exceptions automatically.
# Instead it merely sets OF when the results, if interpreted as signed numbers,
# would be out of range. When doing arithmetic on signed operands, careful
# programmers and compilers either test OF directly or use the INTO instruction.
#*******************************************************************************

