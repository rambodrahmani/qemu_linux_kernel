#*******************************************************************************
# File: outputb.s
#       Transfers a byte from the AL reigster to the I/O port speficied in the
#       DX register.
#       x64 Assembly implementation for the outputb function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputb
#-------------------------------------------------------------------------------
outputb:
    pushq   %rax
    pushq   %rdx
    movb    %dil, %al
    movw    %si, %dx
    outb    %al, %dx
    popq    %rdx
    popq    %rax
    ret
#*******************************************************************************

