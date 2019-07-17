#*******************************************************************************
# File: sti.s
#       Sets interrupt flag; external, maskable interrupts enabled at the end of
#       the next instruction. If protected-mode virtual interrupts are not
#       enabled, STI sets the interrupt flag (IF) in the EFLAGS register. After
#       the IF flag is set, the processor begins responding to external,
#       maskable interrupts after the next instruction is executed. The delayed
#       effect of this instruction is provided to allow interrupts to be enabled
#       just before returning from a procedure (or subroutine). For instance, if
#       an STI instruction is followed by an RET instruction, the RET
#       instruction is allowed to execute before external interrupts are
#       recognized.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 16/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.GLOBAL sti
#-------------------------------------------------------------------------------
sti:
    sti
    ret
#*******************************************************************************

