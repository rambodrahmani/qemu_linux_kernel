#*******************************************************************************
# File: inputl.s
#       Transfers a long from the I/O port specified in the DX register into the
#       EAX register.
#       x32 Assembly implementation for the inputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputl
#-------------------------------------------------------------------------------
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
#*******************************************************************************

