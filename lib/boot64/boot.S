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
.GLOBAL  _start, start                                 # Boot Module Entry point
#-------------------------------------------------------------------------------
_start:
start:
    jmp  multiboot_entry        # scavalchiamo la struttra richiesta
                                # dal bootstrap loader, che deve
                                # trovarsi verso l inizio del file

.align  4
// le seguenti informazioni sono richieste dal bootstrap loader
multiboot_header:
.long   MULTIBOOT_HEADER_MAGIC          # magic number
.long   MULTIBOOT_HEADER_FLAGS          # flag
.long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) // checksum
.long   multiboot_header
.long   _start
.long   _edata
.long   _end
.long   multiboot_entry

multiboot_entry:
    cli                                     # clear interrupt flag
    movl    $(stack + STACK_SIZE), %esp     # initialize stack

    pushl   %ebx            # parametri passati dal loader
    pushl   %eax
    call    cmain           # call C++ initialization method

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
.GLOBAL enable_memory_paging
#-------------------------------------------------------------------------------
# Creates virtual memory tables and enables memory paging.
#-------------------------------------------------------------------------------
enable_memory_paging:
    cli

# Paging tables allocations:
# 0x1000   lev4
#              | 
# 0x2000       +------>lev3
#                      |
# 0x3000       +------>lev2_1
#                      |     |
# 0x7000               |     +------>lev1
#                      |
# 0x4000               +------>lev2_2
#                      |
# 0x5000               +------>lev2_3
#                      |
# 0x6000               +------>lev2_4

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

	# table 1:
	# first entry left empty to handle null pointers dereferencing (*NULL)
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

    # disable cache write-back for video memory addresses
	orb $0x08, 0x7000+0xb8*8

	movl 8(%esp), %eax
	movl %eax, wait_for_gdb
	movl 4(%esp), %eax
	subl $jmp_off_rel, %eax
	movl %eax, jmp_off

    movl %cr4, %eax
    orl $0x00000020, %eax   # cr4.pae, [1]
    movl %eax, %cr4

    movl $0xC0000080, %ecx  # efer msr, [2]
    rdmsr
    orl $0x00000100, %eax   # efer.lme, [2]
    wrmsr

    movl %cr0, %eax
    orl $0x80000000, %eax   # enable paging, [0]
    movl %eax, %cr0

    lgdt gdt64_ptr
    ljmp $8, $long_mode     # enable long mode

#-------------------------------------------------------------------------------
# Enables x86-64 architecture long mode.
# In the x86-64 computer architecture, long mode is the mode where a 64-bit
# operating system can access 64-bit instructions and registers. 64-bit programs
# are run in a sub-mode called 64-bit mode, while 32-bit programs and 16-bit
# protected mode programs are executed in a sub-mode called compatibility mode.
# Real mode or virtual 8086 mode programs cannot be natively run in long mode.
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
#   mov $0x1F201F201F201F20, %rax     
#   mov $0xB8000, %edi
#   mov $500, %rcx            
#   rep stosq                        
#
#   mov $0x00b8000, %edi
#
#   mov $0x1F6C1F6C1F651F48, %rax
#   movq %rax, (%edi)
#
#   mov $0x1F6F1F571F201F6F, %rax
#   movq %rax, 8(%edi)
#
#   mov $0x1F211F641F6C1F72, %rax
#   mov %rax, 16(%edi)

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

#-------------------------------------------------------------------------------
# [0]
# CR0
# The CR0 register is 32 bits long on the 386 and higher processors. On x86-64
# processors in long mode, it (and the other control registers) is 64 bits long.
# CR0 has various control flags that modify the basic operation of the
# processor.
#
# Bit   Name    Full Name               Description
# 0     PE      Protected Mode Enable   If 1, system is in protected mode, else
#                                       system is in real mode.
# 1     MP      Monitor co-processor    Controls interaction of WAIT/FWAIT
#                                       instructions with TS flag in CR0.
# 2     EM      Emulation               If set, no x87 floating-point unit
#                                       present, if clear, x87 FPU present.
# 3     TS      Task switched           Allows saving x87 task context upon a
#                                       task switch only after x87 instruction
#                                       used.
# 4     ET      Extension type          On the 386, it allowed to specify
#                                       whether the external math coprocessor
#                                       was an 80287 or 80387.
# 5     NE      Numeric error           Enable internal x87 floating point error
#                                       reporting when set, else enables PC
#                                       style x87 error detection.
# 16    WP      Write protect           When set, the CPU can't write to
#                                       read-only pages when privilege level is
#                                       0.
# 18    AM      Alignment mask          Alignment check enabled if AM set, AC
#                                       flag (in EFLAGS register) set, and
#                                       privilege level is 3.
# 29    NW      Not-write through       Globally enables/disable write-through
#                                       caching.
# 30    CD      Cache disable           Globally enables/disable the memory
#                                       cache.
# 31    PG      Paging                  If 1, enable paging and use the %CR3
#                                       register, else disable paging.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# [1]
# CR4
# Used in protected mode to control operations such as virtual-8086 support,
# enabling I/O breakpoints, page size extension and machine-check exceptions.
#
# Bit   Name        Full Name            Description
# 0     VME         Virtual 8086 Mode    If set, enables support for the virtual
#                   Extensions           interrupt flag (VIF) in virtual-8086
#                                        mode.
#
# 1     PVI         Protected-mode       If set, enables support for the virtual
#                   Virtual Interrupts   interrupt flag (VIF) in protected mode.
#
# 2     TSD         Time Stamp Disable   If set, RDTSC instruction can only be
#                                        executed when in ring 0, otherwise
#                                        RDTSC can be used at any privilege
#                                        level.
#
# 3     DE          Debugging Extensions If set, enables debug register based
#                                        breaks on I/O space access.
#
# 4     PSE         Page Size Extension  If unset, page size is 4 KiB, else page
#                                        size is increased to 4 MiB;
#                                        If PAE is enabled or the processor is
#                                        in x86-64 long mode this bit is
#                                        ignored.
#
# 5     PAE         Physical Address     If set, changes page table layout to
#                   Extension            translate 32-bit virtual addresses into
#                                        extended 36-bit physical addresses.
#
# 6     MCE         Machine Check        If set, enables machine check
#                   Exception            interrupts to occur.
#
# 7     PGE         Page Global Enabled  If set, address translations (PDE or
#                                        PTE records) may be shared between
#                                        address spaces.
#
# 8     PCE         Performance-         If set, RDPMC can be executed at any
#                   Monitoring counter   privilege level, else RDPMC can only be
#                   enable               used in ring 0.
#
# 9     OSFXSR      Operating system     If set, enables Streaming SIMD
#                   support for          Extensions (SSE) instructions and fast
#                   FXSAVE and FXRSTOR   FPU save & restore.
#                   instructions
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# [2]
# EFER
# Extended Feature Enable Register (EFER) is a model-specific register added in
# the AMD K6 processor, to allow enabling the SYSCALL/SYSRET instruction, and
# later for entering and exiting long mode. This register becomes architectural
# in AMD64 and has been adopted by Intel. Its MSR number is 0xC0000080.
#
# Bit    Purpose
# 0      SCE (System Call Extensions)
# 1–7    Reserved
# 8      LME (Long Mode Enable)
# 9      Reserved
# 10     LMA (Long Mode Active)
# 11     NXE (No-Execute Enable)
# 12     SVME (Secure Virtual Machine Enable)
# 13     LMSLE (Long Mode Segment Limit Enable)
# 14     FFXSR (Fast FXSAVE/FXRSTOR)
# 15     TCE (Translation Cache Extension)
# 16–63  Reserved 
#-------------------------------------------------------------------------------

