#*******************************************************************************
# File: coproc_so.s
#       Coprocessor segment overrun.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL coproc_so
#-------------------------------------------------------------------------------
coproc_so:
    movq  $9, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 9 -- Coprocessor Segment Overrun
# This exception is raised in protected mode if the 80386 detects a page or
# segment violation while transferring the middle portion of a coprocessor
# operand to the NPX. This exception is avoidable.
#*******************************************************************************

