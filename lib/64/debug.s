#*******************************************************************************
# File: debug.s
#       Single-step interrupt.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL debug
#-------------------------------------------------------------------------------
debug:
    movq  $1, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 1 -- Debug Exceptions
# The processor triggers this interrupt for any of a number of conditions;
# whether the exception is a fault or a trap depends on the condition:
#
# - Instruction address breakpoint fault.
# - Data address breakpoint trap.
# - General detect fault.
# - Single-step trap.
# - Task-switch breakpoint trap.
#
# The processor does not push an error code for this exception. An exception
# handler can examine the debug registers to determine which condition caused
# the exception.
#*******************************************************************************

