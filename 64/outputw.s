###
#
# File: outputw.s
#       x64 Assembly implementation for the outputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global outputw

outputw:
    pushq   %rax
    pushq   %rdx
    movw    %di, %ax
    movw    %si, %dx
    outw    %ax, %dx
    popq    %rdx
    popq    %rax
    ret

