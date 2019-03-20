###
#
# File: inputw.s
#       x64 Assembly implementation for the inputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global inputw

inputw:
    pushq   %rax
    pushq   %rdx
    movw    %di, %dx
    inw     %dx, %ax
    movw    %ax, (%rsi)
    popq    %rdx
    popq    %rax
    ret

