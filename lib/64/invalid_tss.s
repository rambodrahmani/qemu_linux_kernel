#*******************************************************************************
# File: invalid_tss.s
#       Bad TSS exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL invalid_tss
#-------------------------------------------------------------------------------
invalid_tss:
    movq  $10, %rdi
    popq  %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 10 -- Invalid TSS
# Interrupt 10 occurs if during a task switch the new TSS is invalid. A TSS is
# considered invalid in the cases shown in Table 9-5. An error code is pushed
# onto the stack to help identify the cause of the fault. The EXT bit indicates
# whether the exception was caused by a condition outside the control of the
# program; e.g., an external interrupt via a task gate triggered a switch to an
# invalid TSS.
# 
# This fault can occur either in the context of the original task or in the
# context of the new task. Until the processor has completely verified the
# presence of the new TSS, the exception occurs in the context of the original
# task. Once the existence of the new TSS is verified, the task switch is
# considered complete; i.e., TR is updated and, if the switch is due to a CALL
# or interrupt, the backlink of the new TSS is set to the old TSS. Any errors
# discovered by the processor after this point are handled in the context of the
# new task.
# 
# To insure a proper TSS to process it, the handler for exception 10 must be a
# task invoked via a task gate.
# 
# Table 9-5. Conditions That Invalidate the TSS
# 
# Error Code              Condition
# 
# TSS id + EXT            The limit in the TSS descriptor is less than 103
# LTD id + EXT            Invalid LDT selector or LDT not present
#  SS id + EXT            Stack segment selector is outside table limit
#  SS id + EXT            Stack segment is not a writable segment
#  SS id + EXT            Stack segment DPL does not match new CPL
#  SS id + EXT            Stack segment selector RPL < >  CPL
#  CS id + EXT            Code segment selector is outside table limit
#  CS id + EXT            Code segment selector does not refer to code
#                         segment
#  CS id + EXT            DPL of non-conforming code segment < > new CPL
#  CS id + EXT            DPL of conforming code segment > new CPL
# DS/ES/FS/GS id + EXT    DS, ES, FS, or GS segment selector is outside
#                         table limits
# DS/ES/FS/GS id + EXT    DS, ES, FS, or GS is not readable segment
#*******************************************************************************

