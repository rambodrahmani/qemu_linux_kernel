################################################################################
# File: outputbw.s
#       Transfers a sequence of words from the memory location specified in
#       DS:(E)SI to the I/O port specified in DX.
#       x32 Assembly implementation for the outputbw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL outputsw
#-------------------------------------------------------------------------------
outputsw:
    pushl   %eax
    pushl   %edx
    pushl   %esi
    pushl   %ecx

    movw    28(%esp), %dx
    movl    20(%esp), %esi
    movl    24(%esp),%ecx
    cld
    rep
    outsw

    popl    %ecx
    popl    %esi
    popl    %edx
    popl    %eax
    ret

