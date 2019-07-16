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

