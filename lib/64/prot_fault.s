#*******************************************************************************
# File: prot_fault.s
#       General protection fault.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 16/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL prot_fault
#-------------------------------------------------------------------------------
prot_fault:
    movq  $13, %rdi
    popq  %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 13 -- General Protection Exception
# All protection violations that do not cause another exception cause a general
# protection exception. This includes (but is not limited to):
#
# 1.  Exceeding segment limit when using CS, DS, ES, FS, or GS;
# 2.  Exceeding segment limit when referencing a descriptor table;
# 3.  Transferring control to a segment that is not executable;
# 4.  Writing into a read-only data segment or into a code segment;
# 5.  Reading from an execute-only segment;
# 6.  Loading the SS register with a read-only descriptor (unless the selector
#     comes from the TSS during a task switch, in which case a TSS exception
#     occurs;
# 7.  Loading SS, DS, ES, FS, or GS with the descriptor of a system segment;
# 8.  Loading DS, ES, FS, or GS with the descriptor of an executable segment
#     that is not also readable;
# 9.  Loading SS with the descriptor of an executable segment;
# 10. Accessing memory via DS, ES, FS, or GS when the segment register contains
#     a null selector;
# 11. Switching to a busy task;
# 12. Violating privilege rules;
# 13. Loading CR0 with PG=1 and PE=0;
# 14. Interrupt or exception via trap or interrupt gate from V86 mode to
#     privilege level other than zero;
# 15. Exceeding the instruction length limit of 15 bytes (this can occur only if
#     redundant prefixes are placed before an instruction).
#
# The general protection exception is a fault. In response to a general
# protection exception, the processor pushes an error code onto the exception
# handler's stack. If loading a descriptor causes the exception, the error code
# contains a selector to the descriptor; otherwise, the error code is null. The
# source of the selector in an error code may be any of the following:
# 
# 1. An operand of the instruction;
# 2. A selector from a gate that is the operand of the instruction;
# 3. A selector from a TSS involved in a task switch. 
#*******************************************************************************

