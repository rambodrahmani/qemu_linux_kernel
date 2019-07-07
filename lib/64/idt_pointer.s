################################################################################
# File: idt_pointer.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.data
.balign 16
.global idt_pointer
idt_pointer:
        .word 0xFFF                     // limite della IDT (256 entrate)
        .quad idt                       // base della IDT
