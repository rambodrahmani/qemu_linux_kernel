#*******************************************************************************
# File: outputbw.s
#       Transfers a sequence of words from the memory location specified in
#       DS:(E)SI to the I/O port specified in DX.
#       x64 Assembly implementation for the outputbw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputsw
#-------------------------------------------------------------------------------
outputsw:
    pushq   %rax
    pushq   %rdx
    pushq   %rsi
    pushq   %rcx

    movq    %rsi, %rcx
    movq    %rdi, %rsi
    cld
    rep
    outsw

    popq    %rcx
    popq    %rsi
    popq    %rdx
    popq    %rax
    ret
#*******************************************************************************

