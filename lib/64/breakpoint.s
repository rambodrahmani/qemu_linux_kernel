#*******************************************************************************
# File: breakpoint.s
#       Breakpoint exception.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL breakpoint
#-------------------------------------------------------------------------------
breakpoint:
    movq  $3, %rdi
    movq  $0, %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 3 -- Breakpoint
# The INT 3 instruction causes this trap. The INT 3 instruction is one byte
# long, which makes it easy to replace an opcode in an executable segment with
# the breakpoint opcode. The operating system or a debugging subsystem can use a
# data-segment alias for an executable segment to place an INT 3 anywhere it is
# convenient to arrest normal execution so that some sort of special processing
# can be performed. Debuggers typically use breakpoints as a way of displaying
# registers, variables, etc., at crucial points in a task.
# 
# The saved CS:EIP value points to the byte following the breakpoint. If a
# debugger replaces a planted breakpoint with a valid opcode, it must subtract
# one from the saved EIP value before returning.
#*******************************************************************************

