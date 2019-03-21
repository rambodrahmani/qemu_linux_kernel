###
#
# File: inputbw.s
#       x32 Assembly implementation for the inputbw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#
###

.text
    .global inputbw

inputbw:
    pushl   %eax
    pushl   %edx
    pushl   %edi
    pushl   %ecx

    movw    20(%esp), %dx
    movl    24(%esp), %edi
    movl    28(%esp), %ecx
    cld
    rep
    insw

    popl    %ecx
    popl    %edi
    popl    %edx
    popl    %eax
    ret

