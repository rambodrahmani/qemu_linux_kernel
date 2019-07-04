################################################################################
# File: outputw.s
#       Transfers a word from the AX reigster to the I/O port speficied in the
#       DX register.
#       x64 Assembly implementation for the outputw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputw
#-------------------------------------------------------------------------------
outputw:
    pushq   %rax
    pushq   %rdx
    movw    %di, %ax
    movw    %si, %dx
    outw    %ax, %dx
    popq    %rdx
    popq    %rax
    ret

