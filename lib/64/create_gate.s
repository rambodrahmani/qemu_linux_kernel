#*******************************************************************************
# File: create_gate.s
#       Creates an IDT gate at the given index for the given subroutine.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL create_gate
#-------------------------------------------------------------------------------
create_gate:
    movq  %rsi, %rax                    # subroutine address

    movw  %ax, (%rdi)                   # 16 bits of the subroutine address
    movw  $SEL_CODICE_SISTEMA, 2(%rdi)

    movw   $0, %ax
    movb   $0b10001110, %ah             # access byte

    andq   $0x1, %rdx
    orb    %dl, %ah
    movb   $0, %al                      # lower half is equal to zero
    movl   %eax, 4(%rdi)                # write 16 most significant bits
                                        # and the access byte
    shrq   $32, %rax                    # extend to 64 bits
    movl   %eax, 8(%rdi)
    movl   $0, 12(%rdi)                 # reserved

    ret
#*******************************************************************************

#*******************************************************************************
# Each IDT entry is similar to that of a GDT entry. Both have hold a base
# address, both hold an access flag, and both are 64-bits long. The major
# differences in these two types of descriptors is in the meanings of these
# fields. In an IDT, the base address specified in the descriptor is actually
# the address of the Interrupt Service Routine that the processor should call
# when this interrupt is 'raised' (called). An IDT entry doesn't have a limit,
# instead it has a segment that you need to specify. The segment must be the
# same segment that the given ISR is located in. This allows the processor to
# give control to the kernel through an interrupt that has occured when the
# processor is in a different ring (like when an application is running).
# 
# The access flags of an IDT entry are also similar to a GDT entry's. There is a
# field to say if the descriptor is actually present or not. There is a field
# for the Descriptor Privilege Level (DPL) to say which ring is the highest
# number that is allowed to use the given interrupt. The major difference is the
# rest of the access flag definition. The lower 5-bits of the access byte is
# always set to 01110 in binary. This is 14 in decimal.
#
# Here is a table to give you a better graphical representation of the access
# byte for an IDT entry:
#
#   7   6    5 4             0
#   P   DPL    Always 01110 (14)
#
# P - Segment is present? (1 = Yes)
# DPL - Which Ring (0 to 3)
#*******************************************************************************

