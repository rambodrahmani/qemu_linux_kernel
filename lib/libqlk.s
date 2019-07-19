#*******************************************************************************
# File: libqlk.s
#       This file contains Assembly library methods.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
# Pushes all registers to the stack.
#-------------------------------------------------------------------------------
.macro save_registers
    pushq   %rax
    pushq   %rcx
    pushq   %rdx
    pushq   %rbx
    pushq   %rsi
    pushq   %rdi
    pushq   %rbp
    pushq   %r8
    pushq   %r9
    pushq   %r10
    pushq   %r11
    pushq   %r12
    pushq   %r13
    pushq   %r14
    pushq   %r15
.endm

#-------------------------------------------------------------------------------
# Pops all registers from stack.
#-------------------------------------------------------------------------------
.macro restore_registers
    popq    %r15
    popq    %r14
    popq    %r13
    popq    %r12
    popq    %r11
    popq    %r10
    popq    %r9
    popq    %r8
    popq    %rbp
    popq    %rdi
    popq    %rsi
    popq    %rbx
    popq    %rdx
    popq    %rcx
    popq    %rax
.endm
#*******************************************************************************

