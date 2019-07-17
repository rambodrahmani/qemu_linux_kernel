#*******************************************************************************
# File: coproc_na.s
#       No coprocessor exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL coproc_na
#-------------------------------------------------------------------------------
coproc_na:
    movq  $7, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 7 -- Coprocessor Not Available
# This exception occurs in either of two conditions:
#
# - The processor encounters an ESC (escape) instruction, and the EM (emulate)
#   bit of CR0 (control register zero) is set.
# - The processor encounters either the WAIT instruction or an ESC instruction,
#   and both the MP (monitor coprocessor) and TS (task switched) bits of CR0 are
#   set.
#*******************************************************************************

