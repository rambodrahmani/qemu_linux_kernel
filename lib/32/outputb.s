################################################################################
# File: outputb.s
#       Transfers a byte from the AL reigster to the I/O port speficied in the
#       DX register.
#       x32 Assembly implementation for the outputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputb
#-------------------------------------------------------------------------------
outputb:
    pushl   %eax
    pushl   %edx
    movb    12(%esp), %al
    movl    16(%esp), %edx
    outb    %al, %dx
    popl    %edx
    popl    %eax
    ret

