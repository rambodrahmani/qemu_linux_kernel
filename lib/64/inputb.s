#*******************************************************************************
# File: inputb.s
#       Transfers a byte from the I/O port speficied in the DX register into the
#       AL register.
#       x64 Assembly implementation for the inputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputb
#-------------------------------------------------------------------------------
inputb:
    pushq   %rax
    pushq   %rdx
    movw    %di, %dx
    inb     %dx, %al
    movb    %al, (%rsi)
    popq    %rdx
    popq    %rax
    ret
#*******************************************************************************

