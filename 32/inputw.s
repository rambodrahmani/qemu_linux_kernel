###
#
# File: inputw.s
#       x32 Assembly implementation for the inputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global inputw

inputw:
    pushl   %eax
    pushl   %edx
    movl    12(%esp), %edx
    inw     %dx, %ax
    movl    16(%esp), %edx
    movw    %ax, (%edx)
    popl    %edx
    popl    %eax
    ret

