#*******************************************************************************
# File: stack_fault.s
#       Stack fault.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 16/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL stack_fault
#-------------------------------------------------------------------------------
stack_fault:
    movq  $12, %rdi
    popq  %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 12 -- Stack Exception
# A stack fault occurs in either of two general conditions:
#
# - As a result of a limit violation in any operation that refers to the SS
#   register. This includes stack-oriented instructions such as POP, PUSH,
#   ENTER, and LEAVE, as well as other memory references that implicitly use SS
#   (for example, MOV AX, [BP+6]). ENTER causes this exception when the stack is
#   too small for the indicated local-variable space.
# - When attempting to load the SS register with a descriptor that is marked
#   not-present but is otherwise valid. This can occur in a task switch, an
#   interlevel CALL, an interlevel return, an LSS instruction, or a MOV or POP
#   instruction to SS.
#
# When the processor detects a stack exception, it pushes an error code onto the
# stack of the exception handler. If the exception is due to a not-present stack
# segment or to overflow of the new stack during an interlevel CALL, the error
# code contains a selector to the segment in question (the exception handler can
# test the present bit in the descriptor to determine which exception occurred);
# otherwise the error code is zero.
#*******************************************************************************

