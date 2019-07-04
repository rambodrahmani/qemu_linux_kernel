################################################################################
# File: inputw.s
#       Transfers a word from the I/O port specified in the DX register into the
#       AX register.
#       x64 Assembly implementation for the inputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputw
#-------------------------------------------------------------------------------
inputw:
    pushq   %rax
    pushq   %rdx
    movw    %di, %dx
    inw     %dx, %ax
    movw    %ax, (%rsi)
    popq    %rdx
    popq    %rax
    ret

