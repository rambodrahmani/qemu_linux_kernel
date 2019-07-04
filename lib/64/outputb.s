###
# File: outputb.s
#       x64 Assembly implementation for the outputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
###

.text
    .global outputb

outputb:
    pushq   %rax
    pushq   %rdx
    movb    %dil, %al
    movw    %si, %dx
    outb    %al, %dx
    popq    %rdx
    popq    %rax
    ret

