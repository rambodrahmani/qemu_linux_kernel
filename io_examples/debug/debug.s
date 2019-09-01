#*******************************************************************************
# File: debug.s
#       For this I/O example we will use Assembly to implement the functions
#       related to enabling and disabling the single step mode for the CPU and
#       the primitive for Interrupt 1.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 31/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../lib/libqlk.s"
#-------------------------------------------------------------------------------
# The 8086 has no instruction to directly set or reset the trap flag. These
# operations are done by pushing the flag register on the stack, changing the
# trap flag bit to what the programmer wants it to be, and then popping the flag
# register back off the stack. The instructions to set the trap flag are: 
.GLOBAL enable_single_step          # Enables single-step mode
#-------------------------------------------------------------------------------
enable_single_step:
    pushf                   # Push Flag Register Onto Stack
    orw $0x0100, (%rsp)     # Set TF Flag
    popf                    # Pop Stack into Flag
    ret

#-------------------------------------------------------------------------------
# To reset the trap flag, simply replace the OR instruction in the preceding
# sequence with the instruction: 
.GLOBAL disable_single_step         # Disables single-step mode
#-------------------------------------------------------------------------------
disable_single_step:
    pushf                   # Push Flag Register Onto Stack
    andw $0xFEFF, (%rsp)    # Unset TF Flag
    popf                    # Pop Stack into Flag
    ret

#-------------------------------------------------------------------------------
.GLOBAL a_debug             # Interrupt 1 primitive: debug exception 
#-------------------------------------------------------------------------------
a_debug:
    save_registers          # save general purpose registers content except %rsp

    movq 120(%rsp), %rdi
    call c_debug

    restore_registers
    iretq
#*******************************************************************************

################################################################################
# Status register:
# 15  14  13  12  11  10  9  8  7  6  5  4  3  2  1  0   (bit position)
# -   -   -   -   O   D   I  T  S  Z  -  A  -  P  -  C   Flags
#                            ^
################################################################################

