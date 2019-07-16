#*******************************************************************************
# File: init_gdt.s
#       Initializes the Global Descriptor Table.
#       The Global Descriptor Table (GDT) is a data structure used by Intel
#       x86-family processors starting with the 80286 in order to define the
#       characteristics of the various memory areas used during program
#       execution, including the base address, the size, and access privileges
#       like executability and writability.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL init_gdt
#-------------------------------------------------------------------------------
init_gdt:
    pushq   %rdx

    movabs  $gdt,%rdx
    addq    $SEL_SYS_TSS, %rdx
    movw    $236, (%rdx)            // [15:0] = limit[15:0]
    movabsq $sys_tss, %rax
    movw    %ax, 2(%rdx)            // [31:16] = base[15:0]
    shrq    $16,%rax
    movb    %al, 4(%rdx)            // [39:32] = base[24:16]
    movb    $0b10001001, 5(%rdx)    // [47:40] = p_dpl_type
    movb    $0, 6(%rdx)             // [55:48] = 0
    movb    %ah, 7(%rdx)            // [63:56] = base[31:24]
    shrq    $16, %rax
    movl    %eax, 8(%rdx)           // [95:64] = base[63:32]
    movl    $0, 12(%rdx)            // [127:96] = 0

    popq    %rdx
    ret
#*******************************************************************************

# Software Initialization for Protected Mode
# ------------------------------------------
# Most of the initialization needed for protected mode can be done either before
# or after switching to protected mode. If done in protected mode, however, the
# initialization procedures must not use protected-mode features that are not
# yet initialized.

# Global Descriptor Table
# -----------------------
# Before any segment register is changed in protected mode, the GDT register
# must point to a valid GDT. Initialization of the GDT and GDTR may be done in
# real-address mode. The GDT (as well as LDTs) should reside in RAM, because the
# processor modifies the accessed bit of descriptors.

