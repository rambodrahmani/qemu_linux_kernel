#*******************************************************************************
# File: outputl.s
#       Transfers a long from the EAX reigster to the I/O port speficied in the
#       DX register.
#       x32 Assembly implementation for the outputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputl
#-------------------------------------------------------------------------------
outputl:
    pushl   %eax
    pushl   %edx
    movl    12(%esp), %eax
    movl    16(%esp), %edx
    outl    %eax, %dx
    popl    %edx
    popl    %eax
    ret
#*******************************************************************************

