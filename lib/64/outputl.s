###
# File: outputl.s
#       x64 Assembly implementation for the outputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
###

.text
    .global outputl

outputl:
    pushq   %rax
    pushq   %rdx
    movl    %edi, %eax
    movw    %si, %dx
    outl    %eax, %dx
    popq    %rdx
    popq    %rax
    ret

