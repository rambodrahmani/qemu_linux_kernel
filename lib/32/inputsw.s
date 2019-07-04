################################################################################
# File: inputsw.s
#       Transfers a sequence of words from the I/O port specified in the DX
#       register into the memory address specified in ES:(E)DI. 
#       x32 Assembly implementation for the inputsw function.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL inputbw
#-------------------------------------------------------------------------------
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

