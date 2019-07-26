#*******************************************************************************
# File: libs.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.MACRO carica_descr num base limite pres dpl tipo gran
    pushl $\gran
    pushl $\tipo
    pushl $\dpl
    pushl $\pres
    pushl $\limite
    pushl $\base
    pushl $\num
    call  init_descrittore
    addl  $28, %esp
.endm

#-------------------------------------------------------------------------------
.DATA
gdt_pointer:
    .word 0xffff    // limite della GDT
    .long gdt       // base della GDT

idt_pointer:
    .word 0x7FF    // limite della IDT (256 entrate)
    .long idt      // base della IDT
.bss
.balign 16

gdt:
    .space 8 * 8192, 0

.balign 16
.global idt

idt:
    .space 8 * 256, 0

save_selectors:
s_CS:   .word 0
s_DS:   .word 0
s_ES:   .word 0
s_FS:   .word 0
s_GS:   .word 0
s_SS:   .word 0

save_gdt:
    .word 0
    .long 0

save_idt:
    .word 0
    .long 0

sys_tss:
    .space 216, 0

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL init_descrittore
#-------------------------------------------------------------------------------
init_descrittore:
    pushl %ebp
    movl  %esp, %ebp

    pushl %ebx
    pushl %ecx
    pushl %eax
    pushl %edx

    movl $gdt, %ebx
    movl 8(%ebp), %ecx              // indice GDT -> %ecx
    movl 16(%ebp), %edx             // limite -> %edx
    movw %dx,  (%ebx, %ecx, 8)      // bit 15:00 limite -> 1a parola descr.
    movw 12(%ebp), %ax              // bit 15:00 base -> %ax
    movw %ax, 2(%ebx, %ecx, 8)      // -> 2a parola descr.
    movb 14(%ebp), %al              // bit 23:16 base -> %al
    orb  24(%ebp), %ah              // DPL
    shlb $5, %ah                    // posizione del DPL nel byte di accesso
    orb  20(%ebp), %ah              // bit di presenza
    orb  28(%ebp), %ah              // tipo
    movw %ax, 4(%ebx, %ecx, 8)      // -> 3a parola descr.
    movb 15(%ebp), %dh              // bit 31:24 base -> %dh
    shrl $16, %edx                  // bit 19:16 limite -> low nibble %dl
    orb  $0b01000000, %dl           // operandi su 32 bit
    orb  32(%ebp), %dl              // granularita
    movw %dx, 6(%ebx, %ecx, 8)      // -> 4a parola descr.

    popl %edx
    popl %eax
    popl %ecx
    popl %ebx

    leave
    ret

#-------------------------------------------------------------------------------
.GLOBAL init_gdt
#-------------------------------------------------------------------------------
init_gdt:
    pushl %ecx
    pushl %eax
    pushl %edi

    movl $gdt, %edi
    movl $0x4000, %ecx
    movl $0, %eax
    rep
    stosl

               //indice     base    limite  P       DPL             TIPO            
    carica_descr    0       0       0       NON_P   0               0               0
    carica_descr    1       0       0xfffff PRES    LIV_SISTEMA     SEG_CODICE      G_PAGINA
    carica_descr    2       0       0xfffff PRES    LIV_SISTEMA     SEG_DATI        G_PAGINA
    carica_descr    3       0       0xfffff PRES    LIV_UTENTE      SEG_CODICE      G_PAGINA
    carica_descr    4       0       0xfffff PRES    LIV_UTENTE      SEG_DATI        G_PAGINA
    carica_descr    5       sys_tss 215     PRES    LIV_SISTEMA     SEG_TSS         G_BYTE
    
    popl %ecx
    popl %eax
    popl %edi

    ret

#-------------------------------------------------------------------------------
.GLOBAL load_gdt
#-------------------------------------------------------------------------------
load_gdt:
    pushl %ecx

    movw    %cs, %cx
    movw    %cx, s_CS
    movw    %ds, %cx
    movw    %cx, s_DS
    movw    %es, %cx
    movw    %cx, s_ES
    movw    %fs, %cx
    movw    %cx, s_FS
    movw    %gs, %cx
    movw    %cx, s_GS
    movw    %ss, %cx
    movw    %cx, s_SS

    sgdt    save_gdt
    lgdt    gdt_pointer    // carichiamo la nuova GDT

    ljmp    $SEL_CODICE_SISTEMA, $qui    // ricarichiamo CS

qui:
    movw    $SEL_DATI_SISTEMA,   %cx    // e gli altri selettori
    movw    %cx, %ss
    movw    %cx, %ds
    movw    %cx, %es
    movw    $0,  %cx    // FS e GS non sono usati
    movw    %cx, %fs
    movw    %cx, %gs

    movw $SEL_SYS_TSS, %cx
    ltr %cx
    
    popl %ecx

    ret

#-------------------------------------------------------------------------------
.GLOBAL unload_gdt
#-------------------------------------------------------------------------------
unload_gdt:
    pushl %ecx

    lgdt  save_gdt

    pushf
    xorl %ecx, %ecx
    movw  s_CS, %cx
    pushl %ecx
    pushl $avanti
    iret
avanti:
    movw    s_DS, %cx
    movw    %cx, %ds
    movw    s_ES, %cx
    movw    %cx, %es
    movw    s_FS, %cx
    movw    %cx, %fs
    movw    s_GS, %cx
    movw    %cx, %gs
    movw    s_SS, %cx
    movw    %cx, %ss

    popl %ecx

    ret

#-------------------------------------------------------------------------------
// Loads an IDT gate at the given index for the given routine
// parameters:
//  num: IDT Gate index (starting from 0)
//  routine: address of the routine
.GLOBAL create_gate
#-------------------------------------------------------------------------------
create_gate:
    pushl %ebp
    movl  %esp, %ebp

    pushl %ebx
    pushl %eax

    movl  8(%ebp), %ebx         // indirizzo struttura
    movl  12(%ebp), %eax        // offset della routine

    movw  %ax, (%ebx)            // primi 16 bit dell offset
    movw  $SEL_CODICE_SISTEMA, 2(%ebx)

    movw  $0, %ax
    movb  $0b10001110, %ah     // byte di accesso
                              // (presente, 32bit, tipo interrupt)
    movb  $LIV_SISTEMA, %al    // DPL
    shlb  $5, %al          // posizione del DPL nel byte di accesso
    orb   %al, %ah         // byte di accesso con DPL in %ah
    movb  $0, %al          // la parte bassa deve essere 0
    movl  %eax, 4(%ebx)    // 16 bit piu sign. dell offset
                          // e byte di accesso

    popl  %eax
    popl  %ebx

    leave
    ret
#-------------------------------------------------------------------------------
# Loads a gate into the IDT.
# Parameters:
#   num:     IDT entry index (starts from 0) to load
#   routine: address of the subroutine to load in the IDT gate
.macro load_gate num routine dpl
	pushl $\routine
	pushl $\num
	call  gate_init
	addl  $8, %esp
.endm
#-------------------------------------------------------------------------------
# Initializes the first 47 entries of the IDT.
.GLOBAL init_idt
#-------------------------------------------------------------------------------
init_idt:
    pushl %ecx
    pushl %eax
    pushl %edi

    movl  $idt, %edi
    movl  $512, %ecx
    movl  $0, %eax
    cld
    rep   stosl

    load_gate 0       divide_error    # Division by zero exception
    load_gate 1       debug           # Single-step interrupt
    load_gate 2       nmi             # Non maskable interrupt
    load_gate 3       breakpoint      # Breakpoint exception
    load_gate 4       overflow        # Overflow exception
    load_gate 5       out_of_bound    # Out of bounds exception
    load_gate 6       invalid_opcode  # Invalid opcode exception
    load_gate 7       coproc_na       # No coprocessor exception
    load_gate 8       double_fault    # Double fault
    load_gate 9       coproc_so       # Coprocessor segment overrun
    load_gate 10      invalid_tss     # Bad TSS
    load_gate 11      segm_fault      # Segment not present
    load_gate 12      stack_fault     # Stack fault
    load_gate 13      prot_fault      # General protection fault
    load_gate 14      page_fault      # Page fault
    load_gate 15      unknown_exc     # Unknown interrupt exception
    load_gate 16      fp_exc          # Coprocessor fault
    load_gate 17      ac_exc          # Alignment check exception
    load_gate 18      mc_exc          # Machine check exception
    load_gate 19      simd_exc
    load_gate 0x20    ignore_pic      # 19-31 - Reserved
    load_gate 0x21    ignore_pic
    load_gate 0x22    ignore_pic
    load_gate 0x23    ignore_pic
    load_gate 0x24    ignore_pic
    load_gate 0x25    ignore_pic
    load_gate 0x26    ignore_pic
    load_gate 0x27    ignore_pic
    load_gate 0x28    ignore_pic
    load_gate 0x29    ignore_pic
    load_gate 0x2A    ignore_pic
    load_gate 0x2B    ignore_pic
    load_gate 0x2C    ignore_pic
    load_gate 0x2D    ignore_pic
    load_gate 0x2E    ignore_pic
    load_gate 0x2F    ignore_pic

    sidt save_idt

    # load the new IDT pointer
    lidt idt_pointer

    popl %edi
    popl %eax
    popl %ecx

    ret

#*******************************************************************************
# Interrupt 0 -- Divide Error
# The divide-error fault occurs during a DIV or an IDIV instruction when the
# divisor is zero.
#*******************************************************************************
divide_error:
    pushl $0
    pushl $0
    jmp   comm_exc

#*******************************************************************************
# Interrupt 1 -- Debug Exceptions
# The processor triggers this interrupt for any of a number of conditions;
# whether the exception is a fault or a trap depends on the condition:
#
# - Instruction address breakpoint fault.
# - Data address breakpoint trap.
# - General detect fault.
# - Single-step trap.
# - Task-switch breakpoint trap.
#
# The processor does not push an error code for this exception. An exception
# handler can examine the debug registers to determine which condition caused
# the exception.
#*******************************************************************************
debug:
    pushl $0
    pushl $1
    jmp   comm_exc

#-------------------------------------------------------------------------------
nmi:
    pushl $0
    pushl $2
    jmp   comm_exc

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
breakpoint:
    pushl $0
    pushl $3
    jmp   comm_exc

#*******************************************************************************
# Interrupt 4 -- Overflow
# This trap occurs when the processor encounters an INTO instruction and the OF
# (overflow) flag is set. Since signed arithmetic and unsigned arithmetic both
# use the same arithmetic instructions, the processor cannot determine which is
# intended and therefore does not cause overflow exceptions automatically.
# Instead it merely sets OF when the results, if interpreted as signed numbers,
# would be out of range. When doing arithmetic on signed operands, careful
# programmers and compilers either test OF directly or use the INTO instruction.
#*******************************************************************************
overflow:
    pushl $0
    pushl $4
    jmp   comm_exc

#*******************************************************************************
# Interrupt 5 -- Bounds Check
# This fault occurs when the processor, while executing a BOUND instruction,
# finds that the operand exceeds the specified limits. A program can use the
# BOUND instruction to check a signed array index against signed limits defined
# in a block of memory.
#*******************************************************************************
out_of_bound:
    pushl $0
    pushl $5
    jmp   comm_exc

#*******************************************************************************
# Interrupt 6 -- Invalid Opcode
# This fault occurs when an invalid opcode is detected by the execution unit.
# (The exception is not detected until an attempt is made to execute the invalid
# opcode: prefetching an invalid opcode does not cause this exception.)
#
# This exception also occurs when the type of operand is invalid for the given
# opcode. Examples include an intersegment JMP referencing a register operand,
# or an LES instruction with a register source operand. 
#*******************************************************************************
invalid_opcode:
    pushl $0
    pushl $6
    jmp   comm_exc

#-------------------------------------------------------------------------------
coproc_na:
    pushl $0
    pushl $7
    jmp   comm_exc

#-------------------------------------------------------------------------------
double_fault:
    pushl $0 // workaround per bochs
    pushl $8
    jmp   comm_exc

#-------------------------------------------------------------------------------
coproc_so:
    pushl $0
    pushl $9
    jmp   comm_exc

#-------------------------------------------------------------------------------
invalid_tss:
    pushl $10
    jmp   comm_exc

#-------------------------------------------------------------------------------
segm_fault:
    pushl $11
    jmp   comm_exc
#-------------------------------------------------------------------------------
stack_fault:
    pushl $12
    jmp   comm_exc

#-------------------------------------------------------------------------------
prot_fault:
    pushl $13
    jmp   comm_exc

#-------------------------------------------------------------------------------
page_fault:
    pushl $14
    jmp   comm_exc

#-------------------------------------------------------------------------------
unknown_exc:
    pushl $0
    pushl $15
    jmp   comm_exc

#-------------------------------------------------------------------------------
fp_exc:
    pushl $0
    pushl $16
    jmp   comm_exc

#-------------------------------------------------------------------------------
ac_exc:
    pushl $17
    jmp   comm_exc

#-------------------------------------------------------------------------------
mc_exc:
    pushl $0
    pushl $18
    jmp   comm_exc

#-------------------------------------------------------------------------------
simd_exc:
    pushl $0
    pushl $19
    jmp   comm_exc

#-------------------------------------------------------------------------------
# Calls the handle_exception subroutine and halts the central processing unit
# (CPU) until the next external interrupt is fired.
#-------------------------------------------------------------------------------
comm_exc:
    call  handle_exception  # call handle_exception
    hlt                     # halt

#-------------------------------------------------------------------------------
ignore_pic:
    call  apic_send_EOI
    iret

#-------------------------------------------------------------------------------
.GLOBAL idt_reset
#-------------------------------------------------------------------------------
idt_reset:
    lidt  save_idt
    ret

#-------------------------------------------------------------------------------
.GLOBAL sti
#       Sets interrupt flag; external, maskable interrupts enabled at the end of
#       the next instruction. If protected-mode virtual interrupts are not
#       enabled, STI sets the interrupt flag (IF) in the EFLAGS register. After
#       the IF flag is set, the processor begins responding to external,
#       maskable interrupts after the next instruction is executed. The delayed
#       effect of this instruction is provided to allow interrupts to be enabled
#       just before returning from a procedure (or subroutine). For instance, if
#       an STI instruction is followed by an RET instruction, the RET
#       instruction is allowed to execute before external interrupts are
#       recognized.
#-------------------------------------------------------------------------------
sti:
    sti
    ret

#-------------------------------------------------------------------------------
.GLOBAL cli
#       Clears interrupt flag; interrupts disabled when interrupt flag cleared.
#       If protected-mode virtual interrupts are not enabled, CLI clears the IF
#       flag in the EFLAGS register. No other flags are affected. Clearing the
#       IF flag causes the processor to ignore maskable external interrupts. The
#       IF flag and the CLI and STI instruction have no affect on the generation
#       of exceptions and NMI interrupts.
#       When protected-mode virtual interrupts are enabled, CPL is 3, and IOPL
#       is less than 3; CLI clears the VIF flag in the EFLAGS register, leaving
#       IF unaffected.
#-------------------------------------------------------------------------------
cli:
    cli
    ret
#*******************************************************************************

