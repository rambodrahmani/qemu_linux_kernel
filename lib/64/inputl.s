#*******************************************************************************
# File: inputl.s
#       Transfers a long from the I/O port specified in the DX register into the
#       EAX register.
#       x64 Assembly implementation for the inputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputl
#-------------------------------------------------------------------------------
inputl:
    pushq   %rax
    pushq   %rdx
    movw    %di, %dx
    inl     %dx, %eax
    movl    %eax, (%rsi)
    popq    %rdx
    popq    %rax
    ret
#*******************************************************************************

