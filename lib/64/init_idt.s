#*******************************************************************************
# File: init_idt.s
#       Initializes the first 47 entries of the IDT.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
# Loads a gate into the IDT.
# Parameters:
#   num:     IDT gate index (starts from 0) to load
#   routine: address of the subroutine to load in the IDT gate

.macro load_gate num routine
	movabsq  $(idt + \num*16), %rdi     # %rdi = IDT gate index address
	movq     $\routine, %rsi            # %rsi = subroutine address
	call     create_gate
.endm
#-------------------------------------------------------------------------------
.GLOBAL init_idt
#-------------------------------------------------------------------------------
init_idt:
    pushq   %rcx
    pushq   %rax
    pushq   %rdi

    movabsq $idt, %rdi
    movq    $256, %rcx
    movq    $0,   %rax
    cld
    rep     stosq

    load_gate 0     divide_error    # Division by zero exception
    load_gate 1     debug           # Single-step interrupt
    load_gate 2     nmi             # Non maskable interrupt
    load_gate 3     breakpoint      # Breakpoint exception
    load_gate 4     overflow        # Overflow exception
    load_gate 5     out_of_bound    # Out of bounds exception
    load_gate 6     invalid_opcode  # Invalid opcode exception
    load_gate 7     coproc_na       # No coprocessor exception
    load_gate 8     double_fault    # Double fault
    load_gate 9     coproc_so       # Coprocessor segment overrun
    load_gate 10    invalid_tss     # Bad TSS
    load_gate 11    segm_fault      # Segment not present
    load_gate 12    stack_fault     # Stack fault
    load_gate 13    prot_fault      # General protection fault
    load_gate 14    page_fault      # Page fault
    load_gate 15    unknown_exc     # Unknown interrupt exception
    load_gate 16    fp_exc          # Coprocessor fault
    load_gate 17    ac_exc          # Alignment check exception
    load_gate 18    mc_exc          # Machine check exception
    load_gate 19    simd_exc        # 19-31 - Reserved
    load_gate 0x20  ignore_pic
    load_gate 0x21  ignore_pic
    load_gate 0x22  ignore_pic
    load_gate 0x23  ignore_pic
    load_gate 0x24  ignore_pic
    load_gate 0x25  ignore_pic
    load_gate 0x26  ignore_pic
    load_gate 0x27  ignore_pic
    load_gate 0x28  ignore_pic
    load_gate 0x29  ignore_pic
    load_gate 0x2A  ignore_pic
    load_gate 0x2B  ignore_pic
    load_gate 0x2C  ignore_pic
    load_gate 0x2D  ignore_pic
    load_gate 0x2E  ignore_pic
    load_gate 0x2F  ignore_pic

    # load new IDT pointer
    lidt    idt_pointer

    popq  %rdi
    popq  %rax
    popq  %rcx

    ret
#*******************************************************************************

#*******************************************************************************
# Faults, traps and exceptions
# The processor will sometimes need to signal your kernel. Something major may
# have happened, such as a divide-by-zero, or a page fault. To do this, it uses
# the first 32 interrupts. It is therefore doubly important that all of these
# are mapped and non-NULL - else the CPU will triple-fault and reset (bochs will
# panic with an 'unhandled exception' error).
#
# The special, CPU-dedicated interrupts are shown below.
# 0 - Division by zero exception
# 1 - Debug exception
# 2 - Non maskable interrupt
# 3 - Breakpoint exception
# 4 - 'Into detected overflow'
# 5 - Out of bounds exception
# 6 - Invalid opcode exception
# 7 - No coprocessor exception
# 8 - Double fault (pushes an error code)
# 9 - Coprocessor segment overrun
# 10 - Bad TSS (pushes an error code)
# 11 - Segment not present (pushes an error code)
# 12 - Stack fault (pushes an error code)
# 13 - General protection fault (pushes an error code)
# 14 - Page fault (pushes an error code)
# 15 - Unknown interrupt exception
# 16 - Coprocessor fault
# 17 - Alignment check exception
# 18 - Machine check exception
# 19-31 - Reserved
#*******************************************************************************

#*******************************************************************************
# Software Initialization for Protected Mode
# ------------------------------------------
# Most of the initialization needed for protected mode can be done either before
# or after switching to protected mode. If done in protected mode, however, the
# initialization procedures must not use protected-mode features that are not
# yet initialized.
#
# Interrupt Descriptor Table
# --------------------------
# The IDTR may be loaded in either real-address or protected mode. However, the
# format of the interrupt table for protected mode is different than that for
# real-address mode. It is not possible to change to protected mode and change
# interrupt table formats at the same time; therefore, it is inevitable that, if
# IDTR selects an interrupt table, it will have the wrong format at some time.
# An interrupt or exception that occurs at this time will have unpredictable
# results. To avoid this unpredictability, interrupts should remain disabled
# until interrupt handlers are in place and a valid IDT has been created in
# protected mode.
#*******************************************************************************

