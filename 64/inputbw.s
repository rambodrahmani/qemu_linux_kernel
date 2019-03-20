###
#
# File: inputbw.s
#       x64 Assembly implementation for the inputbw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global inputbw

inputbw:
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

