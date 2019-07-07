################################################################################
# File: init_idt.s
#       Initializes the first 47 entries of the IDT.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
# Loads a gate into the IDT.
# Parameters:
#   num:     IDT entry index (starts from 0) to load
#   routine: address of the subroutine to load in the IDT gate
.macro load_gate num routine
	movabsq $(idt + \num*16), %rdi
	movq    $\routine, %rsi
	call    componi_gate
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
    movq    $0, %rax
    cld
    rep     stosq

    load_gate 0     divide_error 	
    load_gate 1     debug 		
    load_gate 2     nmi 		
    load_gate 3     breakpoint 	
    load_gate 4     overflow 	
    load_gate 5     bound_re 	
    load_gate 6     invalid_opcode	
    load_gate 7     dev_na 		
    load_gate 8     double_fault 	
    load_gate 9     coproc_so 	
    load_gate 10    invalid_tss 	
    load_gate 11    segm_fault 	
    load_gate 12    stack_fault 	
    load_gate 13    prot_fault 	
    load_gate 14    int_tipo_pf 	
    load_gate 15    unknown_exc	
    load_gate 16    fp_exc 		
    load_gate 17    ac_exc 		
    load_gate 18    mc_exc 		
    load_gate 19    simd_exc 	
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

    lidt    idt_pointer     // carichiamo la nuova IDT

    popq %rdi
    popq %rax
    popq %rcx

    ret

# Software Initialization for Protected Mode
# ------------------------------------------
# Most of the initialization needed for protected mode can be done either before
# or after switching to protected mode. If done in protected mode, however, the
# initialization procedures must not use protected-mode features that are not
# yet initialized.

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

