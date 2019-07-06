################################################################################
# File: inputb.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
// carica un gate nella IDT
// parametri: 
// num: indice (a partire da 0) in IDT del gate da caricare
// routine: indirizzo della routine da associare al gate
.macro carica_gate num routine
	movabsq $(idt + \num*16), %rdi
	movq $\routine, %rsi
	call componi_gate
.endm

.GLOBAL init_idt
init_idt:
    pushq   %rcx
    pushq   %rax
    pushq   %rdi

    movabsq $idt, %rdi
    movq    $256, %rcx
    movq    $0, %rax
    cld
    rep stosq

    carica_gate 0   divide_error 	
    carica_gate	1   debug 		
    carica_gate	2   nmi 		
    carica_gate	3   breakpoint 	
    carica_gate	4   overflow 	
    carica_gate	5   bound_re 	
    carica_gate	6   invalid_opcode	
    carica_gate	7   dev_na 		
    carica_gate	8   double_fault 	
    carica_gate	9   coproc_so 	
    carica_gate	10  invalid_tss 	
    carica_gate	11  segm_fault 	
    carica_gate	12  stack_fault 	
    carica_gate	13  prot_fault 	
    carica_gate	14  int_tipo_pf 	
    carica_gate	15  unknown_exc	
    carica_gate	16  fp_exc 		
    carica_gate	17  ac_exc 		
    carica_gate	18  mc_exc 		
    carica_gate	19  simd_exc 	
    carica_gate	0x20    ignore_pic 	
    carica_gate	0x21    ignore_pic 	
    carica_gate	0x22    ignore_pic 	
    carica_gate	0x23    ignore_pic 	
    carica_gate	0x24    ignore_pic 	
    carica_gate	0x25    ignore_pic 	
    carica_gate	0x26    ignore_pic 	
    carica_gate	0x27    ignore_pic 	
    carica_gate	0x28    ignore_pic 	
    carica_gate	0x29    ignore_pic 	
    carica_gate	0x2A    ignore_pic 	
    carica_gate	0x2B    ignore_pic 	
    carica_gate	0x2C    ignore_pic 	
    carica_gate	0x2D    ignore_pic 	
    carica_gate	0x2E    ignore_pic 	
    carica_gate	0x2F    ignore_pic 	

    lidt    idt_pointer     // carichiamo la nuova IDT

    popq %rdi
    popq %rax
    popq %rcx

    ret

