################################################################################
# File: inputb.s
#       Transfers a byte from the I/O port speficied in the DX register into the
#       AL register.
#       x32 Assembly implementation for the inputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputb
#-------------------------------------------------------------------------------
inputb:
    pushl   %eax
    pushl   %edx
    movl    12(%esp), %edx
    inb     %dx, %al
    movl    16(%esp), %edx
    movb    %al, (%edx)
    popl    %edx
    popl    %eax
    ret

