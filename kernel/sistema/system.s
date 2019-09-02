#*******************************************************************************
# File: system.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 30/09/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "constants.h"

////////////////////////////////////////////////////////////////////////////////
//                         SYSTEM MODULE STARTUP                              //
////////////////////////////////////////////////////////////////////////////////

#-------------------------------------------------------------------------------
# Define stack memory space size.
#define  STACK_SIZE  0x1000

#-------------------------------------------------------------------------------
.GLOBAL _start, start                      # global entry points for the linker
#-------------------------------------------------------------------------------
_start:
start:
# set new address for the stack pointer register:
    movq $(stack + STACK_SIZE), %rsp

    call init_idt                       # initialize IDT

# C++ allows for certain functions to be called before and after the main()
# method. These functions are stored in the __init_array_start array. Since we
# will be compiling our modules with the -nostdlib option enabled, we have to
# call this methods manually because the standard library is not there to take
# care of it.
    movabs $__init_array_start, %rax   # copy first address in %rax
    movq   %rax, %rbx                  # %rax -> %rbx

# loop through the address in __init_array_start:
1:  cmpq   $__init_array_end, %rbx     # check if the last address was reached
    je     2f                          # exit loop
    call   *(%rbx)                     # call method
    addq   $8, %rbx                    # increase %rbx address (next method)
    jmp    1b                          # loop again

# the remaining part of the startup is written in C++:
2:  call   cmain

# execution should never reach this point, if it happens then an error has
# occurred
    hlt


// macro per estrarre la base da un descrittore di TSS
// si aspetta l indirizzo del segmento in %rax e
// lascia il risultato in %rbx
.macro estrai_base
	movl 8(%eax), %ebx
	shlq $32, %rbx      	// bit 63:32 nella parte alta di %rbx
	movb 7(%eax), %bh	// bit 31:24 della base in %bh
	movb 4(%eax), %bl	// bit 23:16 della base in %bl
	shll $16, %ebx		// bit 31:16 nella parte alta di %ebx
	movw 2(%eax), %bx	// bit 15:0 nella parte basse di %ebx
.endm

.macro conv_id_tss
	shlq $4, %rbx
	addq $(des_tss - gdt), %rbx
.endm

// offset dei vari registri all interno di des_proc
.set CR3,104
.set RAX,CR3+8
.set RCX,CR3+16
.set RDX,CR3+24
.set RBX,CR3+32
.set RSP,CR3+40
.set RBP,CR3+48
.set RSI,CR3+56
.set RDI,CR3+64
.set R8, CR3+72
.set R9, CR3+80
.set R10,CR3+88
.set R11,CR3+96
.set R12,CR3+104
.set R13,CR3+112
.set R14,CR3+120
.set R15,CR3+128

// copia lo stato dei registri generali nel des_proc del
// processo puntato da execution.
// Nessun registro viene sporcato.
salva_stato:
	// salviamo lo stato di un paio di registri
	// in modo da poterli temporaneamente riutilizzare
	// In particolare, useremo %rax come registro di lavoro
	// e %rbx come puntatore al des_proc.
	.cfi_startproc
	.cfi_def_cfa_offset 8
	pushq %rbx
	.cfi_adjust_cfa_offset 8
	.cfi_offset rbx, -16
	pushq %rax
	.cfi_adjust_cfa_offset 8
	.cfi_offset rax, -24

	// ricaviamo il puntatore al des_proc
	movq execution,%rax
	movq $0, %rbx
	movw (%rax),%bx		// campo id dal proc_elem
	conv_id_tss		// conversione id -> offset in GDT
	leaq gdt(%rbx), %rax	// indirizzo della corrispondente
				// entrata nella GDT -> %rax
	estrai_base		// base del des_proc -> %rbx

	// copiamo per primo il vecchio valore di %rax
	movq (%rsp), %rax
	movq %rax, RAX(%rbx)
	// usiamo %rax come appoggio per copiare il vecchio %rbx
	movq 8(%rsp), %rax
	movq %rax, RBX(%rbx)
	// copiamo gli altri registri
	movq %rcx, RCX(%rbx)
	movq %rdx, RDX(%rbx)
	// salviamo il valore che %rsp aveva prima della chiamata
	// a salva stato (valore corrente meno gli 8 byte che
	// contengono l'indirizzo di ritorno e i 16 byte dovuti
	// alle due push che abbiamo fatto all'inizio)
	movq %rsp, %rax
	addq $24, %rax
	movq %rax, RSP(%rbx)
	movq %rbp, RBP(%rbx)
	movq %rsi, RSI(%rbx)
	movq %rdi, RDI(%rbx)
	movq %r8,  R8 (%rbx)
	movq %r9,  R9 (%rbx)
	movq %r10, R10(%rbx)
	movq %r11, R11(%rbx)
	movq %r12, R12(%rbx)
	movq %r13, R13(%rbx)
	movq %r14, R14(%rbx)
	movq %r15, R15(%rbx)

	popq %rax
	.cfi_adjust_cfa_offset -8
	.cfi_restore rax
	popq %rbx
	.cfi_adjust_cfa_offset -8
	.cfi_restore rbx

	ret
	.cfi_endproc


// carica nei registri del processore lo stato contenuto nel des_proc del
// processo puntato da execution.
// Questa funzione sporca tutti i registri.
carica_stato:
	.cfi_startproc
	.cfi_def_cfa_offset 8
	// otteniamo la base del des_proc del processo in execution
	// (come per salva_stato)
	movq execution, %rdx
	movq $0, %rbx
	movw (%rdx), %bx
	conv_id_tss
	movq %rbx, %rcx
	leaq gdt(%rbx), %rax
	estrai_base

	// carichiamo TR con l id del nuovo processo
	// (in modo che il meccanismo delle interruzioni usi la
	// pila sistema del nuovo processo)
	andb $0b11111101, 5(%rax)	// reset del bit BUSY
					// (richiesto dal processore
					// per compatibilita* con il modo
					// a 32 bit)
	ltr %cx

	popq %rcx   //ind di ritorno, va messo nella nuova pila
	.cfi_adjust_cfa_offset -8
	.cfi_register rip, rcx

	// nuovo valore per cr3
	movq CR3(%rbx), %r10
	movq %cr3, %rax
	cmpq %rax, %r10
	je 1f			// evitiamo di invalidare il TLB
				// se cr3 non cambia
	movq %r10, %rax
	movq %rax, %cr3		// il TLB viene invalidato
1:

	// anche se abbiamo cambiato cr3 siamo sicuri che
	// l'esecuzione prosegue da qui, perche' ci troviamo dentro
	// la finestra FM che e* comune a tutti i processi
	movq RSP(%rbx), %rsp    //cambiamo pila
	pushq %rcx              //rimettiamo l indirizzo di ritorno
	.cfi_adjust_cfa_offset 8
	.cfi_offset rip, -8

	// se il processo precedente era terminato o abortito la sua pila sistema
	// non era stata distrutta, in modo da permettere a noi di continuare
	// ad usarla. Ora che abbiamo cambiato pila possiamo disfarci della
	// precedente.
	cmpq $0, ultimo_terminato
	je 1f
	call distruggi_pila_precedente
1:

	movq RCX(%rbx), %rcx
	movq RDI(%rbx), %rdi
	movq RSI(%rbx), %rsi
	movq RBP(%rbx), %rbp
	movq RDX(%rbx), %rdx
	movq RAX(%rbx), %rax
	movq R8(%rbx), %r8
	movq R9(%rbx), %r9
	movq R10(%rbx), %r10
	movq R11(%rbx), %r11
	movq R12(%rbx), %r12
	movq R13(%rbx), %r13
	movq R14(%rbx), %r14
	movq R15(%rbx), %r15
	movq RBX(%rbx), %rbx

	retq
	.cfi_endproc


// alloca_tss: usata alla creazione di un processo
// cerca un descrittore di TSS libero, lo inizializza
// ponendo come base l'indirizzo del des_proc passato
// come argomento. Restituisce l'offset del descrittore
// TSS allocato, che funge da identificatore del processo
.set p_dpl_type, 0b10001001 //p=1,dpl=00,type=1001=tss ready
.set pres_bit,   0b10000000
	.global alloca_tss
alloca_tss:
	movq last_tss, %rdx
iter_tss:
	.cfi_startproc
	// usiamo il bit di presenza nel descrittore per
	// distiunguere i descrittori liberi da quelli allocati
	testb $pres_bit, 5(%rdx)
	jz set_entry_tss	// libero, saltiamo all inizializzazione
	addq $16, %rdx		// occupato, passiamo al prossimo
	cmpq $end_gdt, %rdx
	jne advance_tss
	movq $des_tss, %rdx
advance_tss:
	cmpq last_tss, %rdx
	jne iter_tss
	movq $0, %rax		// terminati, restituiamo 0
	jmp end_tss
set_entry_tss:
	movq %rdx, last_tss
	movw $DIM_DESP, (%rdx) 	//[15:0] = limit[15:0]
	decw (%rdx)
	movq %rdi, %rax
	movw %ax, 2(%rdx)	//[31:16] = base[15:0]
	shrq $16,%rax
	movb %al, 4(%rdx)	//[39:32] = base[24:16]
	movb $p_dpl_type, 5(%rdx)	//[47:40] = p_dpl_type
	movb $0, 6(%rdx)	//[55:48] = 0
	movb %ah, 7(%rdx)	//[63:56] = base[31:24]
	shrq $16, %rax
	movl %eax, 8(%rdx) 	//[95:64] = base[63:32]
	movl $0, 12(%rdx)	//[127:96] = 0

	movq %rdx,%rax
	subq $gdt, %rax
end_tss:
	retq
	.cfi_endproc

// rilascia_tss: usata alla terminazione di un processo
// rende nuovamente libero il descrittore TSS associato al processo
// il cui identificatore e* passato come argomento
	.global rilascia_tss
rilascia_tss:
	.cfi_startproc
	movq $0, gdt(%rdi)
	addq $8, %rdi
	movq $0, gdt(%rdi)
	retq
	.cfi_endproc

// dato l identificatore di un processo,
// ne restituisce il puntatore al descrittore
// (0 se non allocato)
	.global des_p
des_p:
	.cfi_startproc
	.cfi_def_cfa_offset 8
	pushq %rbx
	.cfi_adjust_cfa_offset 8
	.cfi_offset rbx, -16
	xorq %rbx, %rbx
	cmpq $0, %rdi
	jl 1f
	cmpq $NUM_TSS, %rdi
	jge 1f
	movq %rdi, %rbx
	conv_id_tss
	leaq gdt(%rbx), %rax
	xorq %rbx, %rbx
	testb $pres_bit, 5(%rax)
	jz 1f
	estrai_base
1:	movq %rbx, %rax
	popq %rbx
	.cfi_adjust_cfa_offset -8
	.cfi_restore rbx
	retq
	.cfi_endproc

	.global id_to_tss
id_to_tss:
	.cfi_startproc
	.cfi_def_cfa_offset 8
	pushq %rbx
	.cfi_adjust_cfa_offset 8
	.cfi_offset rbx, -16
	movq %rdi, %rbx
	conv_id_tss
	movq %rbx, %rax
	popq %rbx
	.cfi_adjust_cfa_offset -8
	.cfi_restore rbx
	ret
	.cfi_endproc

	.global tss_to_id
tss_to_id:
	.cfi_startproc
	movq %rdi, %rax
	subq $(des_tss - gdt), %rax
	shrq $4, %rax
	ret
	.cfi_endproc

// dato un indirizzo virtuale (come parametro) usa l istruzione invlpg per
// eliminare la corrispondente traduzione dal TLB
	.global invalida_entrata_TLB //
invalida_entrata_TLB:
	.cfi_startproc
	invlpg (%rdi)
	ret
	.cfi_endproc

// codice comune alle macro cavallo_di_troia*
violazione:
	movq $2, %rdi
	movabs $param_err, %rsi
	movq %rax, %rdx
	xorq %rax, %rax
	call flog
	call c_abort_p
	call carica_stato
	iretq

// controlla che l indirizzo virtuale op sia accessibile dal
// livello di privilegio del chiamante della INT. Abortisce il
// processo in caso contrario.
.macro cavallo_di_troia reg

	cmpq $SEL_CODICE_SISTEMA, 8(%rsp)
	je 1f
	movabs $0xffff000000000000, %rax
	testq \reg, %rax
	jnz 1f
	movq \reg, %rax
	jmp violazione
1:
.endm

// controlla che base+dim non causi un wrap-around
.macro cavallo_di_troia2 base dim

	movq \base, %rax
	addq \dim, %rax
	jc violazione
.endm

// come sopra, ma la dimensione e* in settori
.macro cavallo_di_troia3 base sec

	movq \base, %rax
	shlq $9, %rax
	addq \sec, %rax
	jc violazione
.endm

#-------------------------------------------------------------------------------
# Loads a gate into the IDT.
# Parameters:
#   num:     IDT gate index (starts from 0) to load
#   routine: address of the subroutine to load in the IDT gate
#   dpl:     Descriptor Priviege Level of the gate

.macro load_gate num routine dpl
    movq  $\num, %rdi
	movq  $\routine, %rsi
    movq  $\dpl, %rdx
	call  init_gate
.endm
#-------------------------------------------------------------------------------
# Initializes the IDT entries. The first 32 entries of the IDT are reserved for
# exceptions. Exception handling is the process of responding to the occurrence,
# during computation, of exceptions – anomalous or exceptional conditions
# requiring special processing – often disrupting the normal flow of program
# execution.
# CPU handles exception the same way it handles external interrupts: each
# exception is identified by a numeric type (from 0 to 31) which is used to
# access an IDT gate where the subroutine handler address is found.
.GLOBAL init_idt
#-------------------------------------------------------------------------------
init_idt:
    //         index     routine         dpl

    // exceptions:
    load_gate  0         divide_error    LEV_SYSTEM
    load_gate  1         debug           LEV_SYSTEM
    load_gate  2         nmi             LEV_SYSTEM
    load_gate  3         breakpoint      LEV_SYSTEM
    load_gate  4         overflow        LEV_SYSTEM
    load_gate  5         bound_re        LEV_SYSTEM
    load_gate  6         invalid_opcode  LEV_SYSTEM
    load_gate  7         dev_na          LEV_SYSTEM
    load_gate  8         double_fault    LEV_SYSTEM
    load_gate  9         coproc_so       LEV_SYSTEM
    load_gate  10        invalid_tss     LEV_SYSTEM
    load_gate  11        segm_fault      LEV_SYSTEM
    load_gate  12        stack_fault     LEV_SYSTEM
    load_gate  13        prot_fault      LEV_SYSTEM
    load_gate  14        int_tipo_pf     LEV_SYSTEM
    load_gate  16        fp_exc          LEV_SYSTEM
    load_gate  17        ac_exc          LEV_SYSTEM
    load_gate  18        mc_exc          LEV_SYSTEM
    load_gate  19        simd_exc        LEV_SYSTEM
    // 19-31 reserved

    // drivers, handlers
    load_gate  VETT_0    driver_td       LEV_SYSTEM
    load_gate  VETT_1    handler_1       LEV_SYSTEM
    load_gate  VETT_2    driver_td       LEV_SYSTEM
    load_gate  VETT_3    handler_3       LEV_SYSTEM
    load_gate  VETT_4    handler_4       LEV_SYSTEM
    load_gate  VETT_5    handler_5       LEV_SYSTEM
    load_gate  VETT_6    handler_6       LEV_SYSTEM
    load_gate  VETT_7    handler_7       LEV_SYSTEM
    load_gate  VETT_8    handler_8       LEV_SYSTEM
    load_gate  VETT_9    handler_9       LEV_SYSTEM
    load_gate  VETT_10   handler_10      LEV_SYSTEM
    load_gate  VETT_11   handler_11      LEV_SYSTEM
    load_gate  VETT_12   handler_12      LEV_SYSTEM
    load_gate  VETT_13   handler_13      LEV_SYSTEM
    load_gate  VETT_14   handler_14      LEV_SYSTEM
    load_gate  VETT_15   handler_15      LEV_SYSTEM
    load_gate  VETT_16   handler_16      LEV_SYSTEM
    load_gate  VETT_17   handler_17      LEV_SYSTEM
    load_gate  VETT_18   handler_18      LEV_SYSTEM
    load_gate  VETT_19   handler_19      LEV_SYSTEM
    load_gate  VETT_20   handler_20      LEV_SYSTEM
    load_gate  VETT_21   handler_21      LEV_SYSTEM
    load_gate  VETT_22   handler_22      LEV_SYSTEM
    load_gate  VETT_23   handler_23      LEV_SYSTEM
    load_gate  VETT_S    handler_24      LEV_SYSTEM

    // primitives
    load_gate  TIPO_A    a_activate_p    LEV_USER
    load_gate  TIPO_T    a_terminate_p   LEV_USER
    load_gate  TIPO_SI   a_sem_ini       LEV_USER
    load_gate  TIPO_W    a_sem_wait      LEV_USER
    load_gate  TIPO_S    a_sem_signal    LEV_USER
    load_gate  TIPO_D    a_delay         LEV_USER
    load_gate  TIPO_L    a_log           LEV_USER

    // I/O primitives
    load_gate  TIPO_APE  a_activate_pe   LEV_SYSTEM
    load_gate  TIPO_WFI  a_wfi           LEV_SYSTEM
    load_gate  TIPO_FG   a_fill_gate     LEV_SYSTEM
    load_gate  TIPO_P    a_panic         LEV_SYSTEM
    load_gate  TIPO_AB   a_abort_p       LEV_SYSTEM
    load_gate  TIPO_TRA  a_trasforma     LEV_SYSTEM

    # load new IDT pointer
    lidt  idt_pointer
    ret

#-------------------------------------------------------------------------------
# Creates and IDT gate at the given index with the given subroutine and DPL.
#  %rdi: IDT entry index;
#  %rsi: subroutine address;
#  %rdx: DPL;
init_gate:
    movq $idt, %r11
    movq %rsi, %rax

    shlq $4, %rdi
    movw %ax, (%r11, %rdi)
    movw $SEL_CODICE_SISTEMA, 2(%r11, %rdi)

    movw $0, %ax
    movb $0b10001110, %ah

    movb %dl, %al
    shlb $5, %al
    orb  %al, %ah
    movb $0, %al
    movl %eax, 4(%r11, %rdi)

    shrq $32, %rax
    movl %eax, 8(%r11,%rdi)
    movl $0, 12(%r11,%rdi)

    ret

#-------------------------------------------------------------------------------
.GLOBAL init_gdt
#-------------------------------------------------------------------------------
init_gdt:
    lgdt gdt_pointer
    retq

////////////////////////////////////////////////////////////////////////////////
//                      a_[primitive] DEFINITIONS                             //
////////////////////////////////////////////////////////////////////////////////

#-------------------------------------------------------------------------------
.extern c_activate_p
#-------------------------------------------------------------------------------
a_activate_p:	// routine int $tipo_a
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	cavallo_di_troia %rdi
        call c_activate_p
	call carica_stato
        iretq
	.cfi_endproc

        .extern c_terminate_p
a_terminate_p:	// routine int $tipo_t
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
        call c_terminate_p
	call carica_stato
	iretq
	.cfi_endproc

	.extern c_sem_ini
a_sem_ini:	// routine int $tipo_si
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call c_sem_ini
	iretq
	.cfi_endproc

	.extern c_sem_wait
a_sem_wait:	// routine int $tipo_w
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call c_sem_wait
	call carica_stato
	iretq
	.cfi_endproc

	.extern c_sem_signal
a_sem_signal:	// routine int $tipo_s
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call c_sem_signal
	call carica_stato
	iretq
	.cfi_endproc

	.extern c_delay
a_delay:	// routine int $tipo_d
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call c_delay
	call carica_stato
	iretq
	.cfi_endproc


////////////////////////////////////////////////////////////////////////////////
//                  INTERFACE AVAILABLE TO THE I/O MODULE                     //
////////////////////////////////////////////////////////////////////////////////

#-------------------------------------------------------------------------------
.EXTERN c_activate_pe
#-------------------------------------------------------------------------------
a_activate_pe:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	cavallo_di_troia %rdi
    call c_activate_pe
	iretq
	.cfi_endproc

#-------------------------------------------------------------------------------
a_wfi:		// routine int $tipo_wfi
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call apic_send_EOI
	call schedule
	call carica_stato
	iretq
	.cfi_endproc

#-------------------------------------------------------------------------------
a_fill_gate:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call init_gate
	iretq
	.cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_panic                            # Interrupt TIPO_P C++ primitive body
#-------------------------------------------------------------------------------
a_panic:                                   # Interrupt TIPO_P primitive
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    //cavallo_di_troia 1
    movq %rsp, %rsi
    call c_panic
1:  nop
    jmp 1b
    .cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_abort_p                         # Interrupt TIPO_AB C++ primitive body
#-------------------------------------------------------------------------------
a_abort_p:                                # Interrupt TIPO_AB primitive
    .cfi_startproc  
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    call c_abort_p
    call carica_stato
    iretq
    .cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_trasforma
#-------------------------------------------------------------------------------
a_trasforma:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call c_trasforma
	iretq
	.cfi_endproc
	.extern c_log

#-------------------------------------------------------------------------------
a_log:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	//cavallo_di_troia 1
	//cavallo_di_troia2 1 2
	call c_log
	call carica_stato
	iretq
	.cfi_endproc

////////////////////////////////////////////////////////////////////////////////
//                              EXCEPTION HANDLING                            //
////////////////////////////////////////////////////////////////////////////////
// All exception are handled the same way: a log message is printed, the current
// process that generated the exception is dumped and aborted. For this reason,
// each exception handler calls the funzione handle_exception (defined in
// system.cpp) passing the exception type number (first argumenti) and the %rip
// which points to the stack top (third argument) which can be used to retrive
// the instruction which generated the exception.
//
// The second argument requires a little bit more of explanations: some
// exceptions leave in the stack an additional quad word, which content depends
// on the exception type. This quad word is remove from the stack and saved into
// exc_error which is then passed as second argument to the handler.
// Those exceptions who do not leave such data in the stack, will simply leave
// 0.
//

#*******************************************************************************
# Interrupt 0 -- Divide Error
# The divide-error fault occurs during a DIV or an IDIV instruction when the
# divisor is zero.
#*******************************************************************************
divide_error:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $0, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $1, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 2 -- Non-maskable interrupt
# In computing, a non-maskable interrupt (NMI) is a hardware interrupt that
# standard interrupt-masking techniques in the system cannot ignore. It
# typically occurs to signal attention for non-recoverable hardware errors.
#*******************************************************************************
nmi:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    call  c_nmi
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $3, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $4, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 5 -- Bounds Check
# This fault occurs when the processor, while executing a BOUND instruction,
# finds that the operand exceeds the specified limits. A program can use the
# BOUND instruction to check a signed array index against signed limits defined
# in a block of memory.
#*******************************************************************************
bound_re:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $5, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $6, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 7 -- Coprocessor Not Available
# This exception occurs in either of two conditions:
#
# - The processor encounters an ESC (escape) instruction, and the EM (emulate)
#   bit of CR0 (control register zero) is set.
# - The processor encounters either the WAIT instruction or an ESC instruction,
#   and both the MP (monitor coprocessor) and TS (task switched) bits of CR0 are
#   set.
#*******************************************************************************
dev_na:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $7, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 8 -- Double Fault
# Normally, when the processor detects an exception while trying to invoke the
# handler for a prior exception, the two exceptions can be handled serially. If,
# however, the processor cannot handle them serially, it signals the
# double-fault exception instead. To determine when two faults are to be
# signalled as a double fault, the 80386 divides the exceptions into three
# classes: benign exceptions, contributory exceptions, and page faults.
#
# Table 9-4 shows which combinations of exceptions cause a double fault and
# which do not.
#
# The processor always pushes an error code onto the stack of the double-fault
# handler; however, the error code is always zero. The faulting instruction may
# not be restarted. If any other exception occurs while attempting to invoke the
# double-fault handler, the processor shuts down.
#
#
# Table 9-3. Double-Fault Detection Classes
#
# Class           ID          Description
#
#                  1          Debug exceptions
#                  2          NMI
#                  3          Breakpoint
# Benign           4          Overflow
# Exceptions       5          Bounds check
#                  6          Invalid opcode
#                  7          Coprocessor not available
#                 16          Coprocessor error
#
#                  0          Divide error
#                  9          Coprocessor Segment Overrun
# Contributory    10          Invalid TSS
# Exceptions      11          Segment not present
#                 12          Stack exception
#                 13          General protection
#
# Page Faults     14          Page fault
#*******************************************************************************
double_fault:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    popq  exc_error
    call  salva_stato
    movq  $8, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 9 -- Coprocessor Segment Overrun
# This exception is raised in protected mode if the 80386 detects a page or
# segment violation while transferring the middle portion of a coprocessor
# operand to the NPX. This exception is avoidable.
#*******************************************************************************
coproc_so:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call  salva_stato
    movq  $9, %rdi
    movq  $0, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------


#*******************************************************************************
# Interrupt 10 -- Invalid TSS
# Interrupt 10 occurs if during a task switch the new TSS is invalid. A TSS is
# considered invalid in the cases shown in Table 9-5. An error code is pushed
# onto the stack to help identify the cause of the fault. The EXT bit indicates
# whether the exception was caused by a condition outside the control of the
# program; e.g., an external interrupt via a task gate triggered a switch to an
# invalid TSS.
# 
# This fault can occur either in the context of the original task or in the
# context of the new task. Until the processor has completely verified the
# presence of the new TSS, the exception occurs in the context of the original
# task. Once the existence of the new TSS is verified, the task switch is
# considered complete; i.e., TR is updated and, if the switch is due to a CALL
# or interrupt, the backlink of the new TSS is set to the old TSS. Any errors
# discovered by the processor after this point are handled in the context of the
# new task.
# 
# To insure a proper TSS to process it, the handler for exception 10 must be a
# task invoked via a task gate.
# 
# Table 9-5. Conditions That Invalidate the TSS
# 
# Error Code              Condition
# 
# TSS id + EXT            The limit in the TSS descriptor is less than 103
# LTD id + EXT            Invalid LDT selector or LDT not present
#  SS id + EXT            Stack segment selector is outside table limit
#  SS id + EXT            Stack segment is not a writable segment
#  SS id + EXT            Stack segment DPL does not match new CPL
#  SS id + EXT            Stack segment selector RPL < >  CPL
#  CS id + EXT            Code segment selector is outside table limit
#  CS id + EXT            Code segment selector does not refer to code
#                         segment
#  CS id + EXT            DPL of non-conforming code segment < > new CPL
#  CS id + EXT            DPL of conforming code segment > new CPL
# DS/ES/FS/GS id + EXT    DS, ES, FS, or GS segment selector is outside
#                         table limits
# DS/ES/FS/GS id + EXT    DS, ES, FS, or GS is not readable segment
#*******************************************************************************
invalid_tss:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop   exc_error
    .cfi_adjust_cfa_offset -8
    call  salva_stato
    movq  $10, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 11 -- Segment Not Present
# Exception 11 occurs when the processor detects that the present bit of a
# descriptor is zero. The processor can trigger this fault in any of these
# cases:
#
# - While attempting to load the CS, DS, ES, FS, or GS registers; loading the SS
#   register, however, causes a stack fault.
# - While attempting loading the LDT register with an LLDT instruction; loading
#   the LDT register during a task switch operation, however, causes the
#   "invalid TSS" exception.
# - While attempting to use a gate descriptor that is marked not-present.
#
# This fault is restartable. If the exception handler makes the segment present
# and returns, the interrupted program will resume execution.
#*******************************************************************************
segm_fault:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop   exc_error
    .cfi_adjust_cfa_offset -8
    call  salva_stato
    movq  $11, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
stack_fault:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop   exc_error
    .cfi_adjust_cfa_offset -8
    call  salva_stato
    movq  $12, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

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
prot_fault:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop   exc_error
    .cfi_adjust_cfa_offset -8
    call  salva_stato
    movq  $13, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  handle_exception
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 14 -- Page Fault
# This exception occurs when paging is enabled (PG=1) and the processor detects
# one of the following conditions while translating a linear address to a
# physical address:
#
# 1. The page-directory or page-table entry needed for the address translation
#    has zero in its present bit;
# 2. The current procedure does not have sufficient privilege to access the
#    indicated page.
#*******************************************************************************
int_tipo_pf:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop  exc_error
    .cfi_adjust_cfa_offset -8
    call  salva_stato
    movq  $14, %rdi
    movq  exc_error, %rsi
    movq  (%rsp), %rdx
    call  c_pre_routine_pf
    call  carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
#*******************************************************************************
.global readCR2
readCR2:
    .cfi_startproc
    movq %cr2, %rax
    retq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 16 --
#*******************************************************************************
fp_exc:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    movq $16, %rdi
    movq $0, %rsi
    movq (%rsp), %rdx
    call handle_exception
    call carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 17 --
#*******************************************************************************
ac_exc:
    .cfi_startproc
    .cfi_def_cfa_offset 48
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    pop exc_error
    .cfi_adjust_cfa_offset -8
    call salva_stato
    movq $13, %rdi
    movq exc_error, %rsi
    movq (%rsp), %rdx
    call handle_exception
    call carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 18 --
#*******************************************************************************
mc_exc:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    movq $18, %rdi
    movq $0, %rsi
    movq (%rsp), %rdx
    call handle_exception
    call carica_stato
    iretq
    .cfi_endproc
#-------------------------------------------------------------------------------

#*******************************************************************************
# Interrupt 19 --
#*******************************************************************************
simd_exc:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    movq $19, %rdi
    movq $0, %rsi
    movq (%rsp), %rdx
    call handle_exception
    call carica_stato
    iretq
    .cfi_endproc
#----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
//                              HANDLERS / DRIVERS                            //
////////////////////////////////////////////////////////////////////////////////

// driver del timer
	.extern c_driver_td
driver_td:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call c_driver_td
	call apic_send_EOI
	call carica_stato

	iretq
	.cfi_endproc

handler_1:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $1, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_2:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $2, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_3:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $3, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_4:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $4, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_5:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $5, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_6:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $6, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_7:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $7, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_8:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $8, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_9:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $9, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_10:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $10, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_11:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $11, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_12:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $12, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_13:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $13, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_14:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $14, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_15:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $15, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_16:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $16, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_17:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $17, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_18:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $18, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_19:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $19, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_20:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $20, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_21:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $21, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_22:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $22, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_23:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $23, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc

handler_24:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato
	call inspronti

	movq $24, %rcx
	movq a_p(, %rcx, 8), %rax
	movq %rax, execution

	call carica_stato
	iretq
	.cfi_endproc


	.global invalida_TLB  //
invalida_TLB:
	.cfi_startproc
	movq %cr3, %rax
	movq %rax, %cr3
	retq
	.cfi_endproc

// carica il registro cr3
// parametri: indirizzo fisico del nuovo direttorio
	.global loadCR3
loadCR3:
	.cfi_startproc
	movq %rdi, %cr3
	retq
	.cfi_endproc

// restituisce in %eax il contenuto di cr3
	.global readCR3
readCR3:
	.cfi_startproc
	movq %cr3, %rax
	retq
	.cfi_endproc


//TIMER
.set CWR,     0x43
.set CTR_LSB, 0x40
.set CTR_MSB, 0x40

// attiva il timer di sistema
// parametri: il valore da caricare nel registro CTR del timer
.extern apic_set_MIRQ
	.global attiva_timer
attiva_timer:
	.cfi_startproc
	movb $0x36, %al
	outb %al, $CWR
	movl %edi, %eax
	outb %al, $CTR_LSB
	movb %ah, %al
	outb %al, $CTR_MSB

	movq $2, %rdi
	movq $0, %rsi
	call apic_set_MIRQ

	retq
	.cfi_endproc

//////////////////////////////////////////////////////////
// primitive richiamate dal nucleo stesso	        //
//////////////////////////////////////////////////////////
	.global sem_ini
sem_ini:
	.cfi_startproc
	int $TIPO_SI
	ret
	.cfi_endproc

	.global sem_wait
sem_wait:
	.cfi_startproc
	int $TIPO_W
	ret
	.cfi_endproc

	.global activate_p
activate_p:
	.cfi_startproc
	int $TIPO_A
	ret
	.cfi_endproc

	.global terminate_p
terminate_p:
	.cfi_startproc
	int $TIPO_T
	ret
	.cfi_endproc

	.global panic
panic:
	.cfi_startproc
	int $TIPO_P
	ret
	.cfi_endproc

	.global salta_a_main
salta_a_main:
	.cfi_startproc
	call carica_stato		// carichiamo tr
	iretq				// torniamo al chiamante "trasformati" in processo
	.cfi_endproc

	.global end_program
end_program:
       lidt triple_fault_idt
       int $1

////////////////////////////////////////////////////////////////////////////////
//                               DATA SECTION                                 //
////////////////////////////////////////////////////////////////////////////////
.DATA
.GLOBAL  fine_codice_sistema
#_------------------------------------------------------------------------------
fine_codice_sistema:
	.QUAD etext

#-------------------------------------------------------------------------------
# GDT memory space location as required by Assembly LGDT instruction: 'The
# source operand specifies a 6-byte memory location that contains the base
# address (a linear address) and the limit (size of table in bytes) of the
# global descriptor table (GDT)'.
gdt_pointer:
    .WORD end_gdt - gdt       # GDT top
    .QUAD gdt               # GDT base

#-------------------------------------------------------------------------------
# IDT memory space location as required by Assembly IDT instruction: 'The source
# operand specifies a 6-byte memory location that contains the base address (a
# linear address) and the limit (size of table in bytes) of the interrupt
# descriptor table (IDT)'.
idt_pointer:
    .WORD 0xFFF             # IDT top: 256 entries
    .QUAD idt               # IDT base

#-------------------------------------------------------------------------------
triple_fault_idt:
    .WORD 0
    .QUAD 0

#-------------------------------------------------------------------------------
param_err:
    .ASCIZ "Invalid address: %p"

#-------------------------------------------------------------------------------
# GDT 
.BALIGN 8
.GLOBAL gdt
#-------------------------------------------------------------------------------
gdt:
    .quad 0             # null segment
code_sys_seg:
	.word 0b0           //limit[15:0]   not used
	.word 0b0           //base[15:0]    not used
	.byte 0b0           //base[23:16]   not used
	.byte 0b10011010    //P|DPL|1|1|C|R|A|  DPL=00=sistema
	.byte 0b00100000    //G|D|L|-|-------|  L=1 long mode
	.byte 0b0           //base[31:24]   not used
code_usr_seg:
	.word 0b0           //limit[15:0]   not used
	.word 0b0           //base[15:0]    not used
	.byte 0b0           //base[23:16]   not used
	.byte 0b11111010    //P|DPL|1|1|C|R|A|  DPL=11=utente
	.byte 0b00100000    //G|D|L|-|-------|  L=1 long mode
	.byte 0b0           //base[31:24]   not used
data_usr_seg:
	.word 0b0           //limit[15:0]   not used
	.word 0b0           //base[15:0]    not used
	.byte 0b0           //base[23:16]   not used
	.byte 0b11110010    //P|DPL|1|0|E|W|A|  DPL=11=utente
	.byte 0b00000000    //G|D|-|-|-------|
	.byte 0b0           //base[31:24]   not used
des_tss:
	.space 16*NUM_TSS,0	//segmento tss, riempito a runtime
end_gdt:

last_tss:
.quad	des_tss

.bss
exc_error:
	.space 8, 0
.balign 16
idt:
	// spazio per 256 gate
	// verra riempita a tempo di esecuzione
	.space 16 * 256, 0
end_idt:

#-------------------------------------------------------------------------------
# Reserve stack memory space
stack:
    .space STACK_SIZE, 0
#*************************************************************************************
