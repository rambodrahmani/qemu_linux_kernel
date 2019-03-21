###
#
# File: inputl.s
#       x32 Assembly implementation for the inputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global inputl

inputl:
    pushl   %eax
    pushl   %edx
    movl    12(%esp), %edx
    inl     %dx, %eax
    movl    16(%esp), %edx
    movl    %eax, (%edx)
    popl    %edx
    popl    %eax
    ret

