#*******************************************************************************
# File: inputsw.s
#       Transfers a sequence of words from the I/O port specified in the DX
#       register into the memory address specified in ES:(E)DI. 
#       x64 Assembly implementation for the inputsw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputsw
#-------------------------------------------------------------------------------
inputsw:
    pushq   %rax
    pushq   %rdx
    pushq   %rdi
    pushq   %rcx

    movq    %rdx, %rcx
    movw    %di, %dx
    movq    %rsi, %rdi
    cld
    rep
    insw

    popq    %rcx
    popq    %rdi
    popq    %rdx
    popq    %rax
    ret
#*******************************************************************************

