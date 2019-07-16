#*******************************************************************************
# File: idt_pointer.s
#		Defines the maximum number of entries in the IDT and a pointer to the
#       IDT base address.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.DATA
.balign 16
#-------------------------------------------------------------------------------
.GLOBAL idt_pointer
idt_pointer:
    .word  0xFFF    # IDT maximum number of entries: 256
    .quad  idt      # IDT pointer
#*******************************************************************************

