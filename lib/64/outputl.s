################################################################################
# File: outputl.s
#       Transfers a long from the EAX reigster to the I/O port speficied in the
#       DX register.
#       x64 Assembly implementation for the outputl function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputl
#-------------------------------------------------------------------------------
outputl:
    pushq   %rax
    pushq   %rdx
    movl    %edi, %eax
    movw    %si, %dx
    outl    %eax, %dx
    popq    %rdx
    popq    %rax
    ret

