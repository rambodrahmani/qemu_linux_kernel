###
#
# File: outputbw.s
#       x64 Assembly implementation for the outputbw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global outputbw

outputbw:
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

