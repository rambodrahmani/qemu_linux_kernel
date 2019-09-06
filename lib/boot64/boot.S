#*******************************************************************************
# File: boot.S
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#define ASM 1
#-------------------------------------------------------------------------------
#                THIS SYSTEM USES THE Multiboot2 STANDARD.
#include "mboot.h"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
#define STACK_SIZE                      0x4000

#ifdef __ELF__
#	define MULTIBOOT_HEADER_FLAGS         (0x00000003)
#else
# 	define MULTIBOOT_HEADER_FLAGS         (0x00010003)
#endif
#-------------------------------------------------------------------------------
.DATA
#-------------------------------------------------------------------------------
.balign 4096
gdt64:
    .quad 0                  // null segment
    .quad 0x209c0000000000   // system code segment

gdt64_end:
gdt64_ptr:
    .word gdt64_end - gdt64
    .long gdt64

.GLOBAL stack
stack:
	.space STACK_SIZE, 0
#-------------------------------------------------------------------------------
.TEXT
.GLOBAL  _start, start
#-------------------------------------------------------------------------------
_start:                         // entry point
start:
    jmp  multiboot_entry        // scavalchiamo la struttra richiesta
                                // dal bootstrap loader, che deve
                                // trovarsi verso l inizio del file

.align  4
// le seguenti informazioni sono richieste dal bootstrap loader
multiboot_header:
.long   MULTIBOOT_HEADER_MAGIC          // valore magico
.long   MULTIBOOT_HEADER_FLAGS          // flag
.long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) // checksum
.long   multiboot_header
.long   _start
.long   _edata
.long   _end
.long   multiboot_entry

multiboot_entry:
    cli                                     # clear interrupt flag
    movl    $(stack + STACK_SIZE), %esp     # initialize stack

    pushl   %ebx            // parametri passati dal loader
    pushl   %eax
    call    cmain           // call C++ initialization method

#-------------------------------------------------------------------------------
# A control register is a processor register which changes or controls the
# general behavior of a CPU or other digital device. Common tasks performed by
# control registers include interrupt control, switching the addressing mode,
# paging control, and coprocessor control.
#
# CR3: Used when virtual addressing is enabled, hence when the PG bit is set in
# CR0. CR3 enables the processor to translate linear addresses into physical
# addresses by locating the page directory and page tables for the current task.
# Typically, the upper 20 bits of CR3 become the page directory base register
# (PDBR), which stores the physical address of the first page directory entry.
# If the PCIDE bit in CR4 is set, the lowest 12 bits are used for the
# process-context identifier (PCID).
#
# Loads a new value in the CR3 register.
.GLOBAL loadCR3
#-------------------------------------------------------------------------------
loadCR3:
	pushl %ebp
	movl  %esp, %ebp
	pushl %eax

	movl  8(%ebp), %eax
	movl  %eax, %cr3
	
	popl  %eax
	leave
	ret

#-------------------------------------------------------------------------------
# Returns the value of CR3 in %eax.
.GLOBAL readCR3
#-------------------------------------------------------------------------------
readCR3:
    movl  %cr3, %eax
    ret

#-------------------------------------------------------------------------------
// attiva la paginazione
.global attiva_paginazione
attiva_paginazione:
	cli

// allochiamo le seguenti tabelle:
// 0x1000	liv4
//              | 
//  0x2000      +------>liv3
//                      |
//   0x3000		+------>liv2_1
//                      |       |
//    0x7000            |       +------>liv1
//                      |
//   0x4000             +------>liv2_2
//                      |
//   0x5000             +------>liv2_3
//                      |
//   0x6000             +------>liv2_4

	movl $0x1000, %ebx // indirizzo di partenza
// azzeriamo tutto
	movl %ebx,  %edi
	movl $(7*1024), %ecx
	movl $0, %eax
	rep stosl
// riempiamo la tabella liv4
	// prima entrata -> tab liv3
	movl $0x2003, (%ebx) // P, R/W

// riempiamo la tabella liv3
	movl $0x2000, %ebx // indirizzo di partenza
	// prima entrata -> tab liv2_1
	movl $0x3003, (%ebx)	// P, R/W
	// seconda entrata -> tab liv2_2
	movl $0x4003, 8(%ebx)	// P, R/W
	// terza entrata -> tab liv2_3
	movl $0x5003, 16(%ebx)  // P, R/W
	// quarta entrata -> tab liv2_4
	movl $0x601b, 24(%ebx)  // P, R/W, PWT, PCD

// riempiamo la tabella liv2_1
	movl $0x3000, %ebx
	// prima entrata -> tab liv1
	movl $0x7003, (%ebx)	// P, R/W
	// entrate successive: traduzione identita'
	addl $8, %ebx
	movl $0x200183, %edx	// 2a entrata: P, R/W, PS, G

ciclo_tab_liv2_1:
	movl %edx, (%ebx)
	addl $0x200000, %edx
	addl $8, %ebx
	cmpl $0x4000, %ebx
	jb ciclo_tab_liv2_1

// riempiamo la tabella liv2_2
// traduzione identita'
ciclo_tab_liv2_2:
	movl %edx, (%ebx)
	addl $0x200000, %edx
	addl $8, %ebx
	cmpl $0x5000, %ebx
	jb ciclo_tab_liv2_2

// riempiamo la tabella liv2_3
// traduzione identita'
ciclo_tab_liv2_3:
	movl %edx, (%ebx)
	addl $0x200000, %edx
	addl $8, %ebx
	cmpl $0x6000, %ebx
	jb ciclo_tab_liv2_3

// riempiamo la tabella liv2_4
// 1GiB di memory-mapped I/O
	movb $0x9b, %dl
ciclo_tab_liv2_4:
	movl %edx, (%ebx)
	addl $0x200000, %edx
	addl $8, %ebx
	cmpl $0x7000, %ebx
	jb ciclo_tab_liv2_4

	// tabella 1:
	// prima entrata non mappata (per intercettare *NULL)
	movl $0, (%ebx)
	// entrate successive: traduzione identita'
	movl $0x1103, %edx	// P, R/W, G
	addl $8, %ebx

ciclo_tab_liv1:
	movl %edx, (%ebx)
	addl $0x1000, %edx
	addl $8, %ebx
	cmpl $0x8000, %ebx
	jb ciclo_tab_liv1

	// disabilitiamo il write-back per gli indirizzi
	// che corrispondono alla memoria video in
	// modalita' testo
	orb $0x08, 0x7000+0xb8*8

	movl 8(%esp), %eax
	movl %eax, wait_for_gdb
	movl 4(%esp), %eax
	subl $jmp_off_rel, %eax
	movl %eax, jmp_off

	movl %cr4, %eax
        orl $0x00000020, %eax  //cr4.pae
        movl %eax, %cr4

        movl $0xC0000080, %ecx   //efer msr
        rdmsr
        orl $0x00000100, %eax   //efer.lme
        wrmsr

        movl %cr0, %eax
        orl $0x80000000, %eax   //cr0.pg
        movl %eax, %cr0
	lgdt gdt64_ptr
	ljmp $8, $long_mode

#-------------------------------------------------------------------------------
.code64
long_mode:
	pushq $0
	popfq
	movw $0, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %fs
	movw %ax, %gs
	#movw $0, %ax
	movw %ax, %ss
#    	mov $0x1F201F201F201F20, %rax     
#	mov $0xB8000, %edi
#    	mov $500, %rcx            
#        rep stosq                        
# 
#        mov $0x00b8000, %edi
# 
#        mov $0x1F6C1F6C1F651F48, %rax
#        movq %rax, (%edi)
# 
#        mov $0x1F6F1F571F201F6F, %rax
#        movq %rax, 8(%edi)
# 
#        mov $0x1F211F641F6C1F72, %rax
#        mov %rax, 16(%edi)

wait:
	cmpl $1, wait_for_gdb(%rip)
	je wait

	.byte 0xe9
jmp_off:
	.long 0
jmp_off_rel:
	nop
.code32
#*******************************************************************************

