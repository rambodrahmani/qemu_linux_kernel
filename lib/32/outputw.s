###
# File: outputw.s
#       x32 Assembly implementation for the outputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
###

.text
    .global outputw

outputw:
    pushl   %eax
    pushl   %edx
    movw    12(%esp), %ax
    movl    16(%esp), %edx
    outw    %ax, %dx
    popl    %edx
    popl    %eax
    ret

