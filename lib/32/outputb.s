###
# File: outputb.s
#       x32 Assembly implementation for the outputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
###

.text
    .global outputb

outputb:
    pushl   %eax
    pushl   %edx
    movb    12(%esp), %al
    movl    16(%esp), %edx
    outb    %al, %dx
    popl    %edx
    popl    %eax
    ret

