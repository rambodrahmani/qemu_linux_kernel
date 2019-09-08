#*******************************************************************************
# File: io.s
#       I/O Module Assembly implementation.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 30/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "constants.h"
#-------------------------------------------------------------------------------
param_err:
	.asciz "indirizzo non valido: %p"

#-------------------------------------------------------------------------------
# Calls fill_gate with the specified arguments. All I/O primitives have user
# level DPL
.macro fill_io_gate gate off
    movq $\gate, %rdi           # set IDT gate index
    movabs $\off, %rax          # set subroutine address
    movq %rax, %rsi
    movq $LEV_USER, %rdx        # set IDT gate DPL
    call fill_gate
.endm

#-------------------------------------------------------------------------------
violazione:
	movq $2, %rdi
	movabs $param_err, %rsi
	movq %rax, %rdx
	xorq %rax, %rax
	call flog
	int $TIPO_AB

#-------------------------------------------------------------------------------
# controlla che l'indirizzo virtuale op sia accessibile dal
# livello di privilegio del chiamante della INT. Abortisce il
# processo in caso contrario.
.macro trojan_horse reg
	cmpq $SEL_CODICE_SISTEMA, 8(%rsp)
	je 1f
	movabs $0xffff000000000000, %rax
	testq \reg, %rax
	jnz 1f
	movq \reg, %rax
	jmp violazione
1:	
.endm

#-------------------------------------------------------------------------------
# controlla che base+dim non causi un wrap-around
.macro trojan_horse2 base dim
	movq \base, %rax
	addq \dim, %rax
	jc violazione
.endm

#-------------------------------------------------------------------------------
# come sopra, ma la dimensione e' in settori
.macro trojan_horse3 base sec
	movq \base, %rax
	shlq $9, %rax
	addq \sec, %rax
	jc violazione
.endm

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _start, start                                   # I/O Module Entry Point
#-------------------------------------------------------------------------------
_start:
start:
    jmp  cmain

////////////////////////////////////////////////////////////////////////////////
//                                  SYSTEM CALLS                              //
////////////////////////////////////////////////////////////////////////////////

#-------------------------------------------------------------------------------
.GLOBAL activate_p
#-------------------------------------------------------------------------------
activate_p:
    .cfi_startproc
	int $TIPO_A
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL terminate_p
#-------------------------------------------------------------------------------
terminate_p:
	.cfi_startproc
	int $TIPO_T
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL sem_ini
#-------------------------------------------------------------------------------
sem_ini:
	.cfi_startproc
	int $TIPO_SI
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL sem_wait
#-------------------------------------------------------------------------------
sem_wait:
	.cfi_startproc
	int $TIPO_W
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL sem_signal
#-------------------------------------------------------------------------------
sem_signal:
	.cfi_startproc
	int $TIPO_S
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL trasforma
#-------------------------------------------------------------------------------
trasforma:
	.cfi_startproc
	int $TIPO_TRA
	ret
	.cfi_endproc

////////////////////////////////////////////////////////////////////////////////
//                      INTERFACE TO THE SYSTEM MODULE                        //
////////////////////////////////////////////////////////////////////////////////
# Every access to the System module through the interrupt mechanism will result
# in a change in the privilege level. Right after the end of the processes
# implemented by the interrupt mechanism, the save_state function
# (system/system.s) jumps in. On the pther hand, leaving the system module using
# the iretq instruction must go through the load_state function
# (system/system.s).

#-------------------------------------------------------------------------------
.GLOBAL activate_pe
#-------------------------------------------------------------------------------
activate_pe:
	.cfi_startproc
	int $TIPO_APE
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL wfi
#-------------------------------------------------------------------------------
wfi:
	.cfi_startproc
	int $TIPO_WFI
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL panic
#-------------------------------------------------------------------------------
panic:
	.cfi_startproc
	int $TIPO_P
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL abort_p
#-------------------------------------------------------------------------------
abort_p:
	.cfi_startproc
	int $TIPO_AB
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL fill_gate                              # Interface to system.s/init_gate
#-------------------------------------------------------------------------------
fill_gate:
    .cfi_startproc
    int $TIPO_FG
    ret
    .cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL delay
#-------------------------------------------------------------------------------
delay:
	.cfi_startproc
	int $TIPO_D
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL do_log
#-------------------------------------------------------------------------------
do_log:
	.cfi_startproc
	int $TIPO_L
	ret
	.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL writevid
#-------------------------------------------------------------------------------
writevid:
	.cfi_startproc
	int $IO_TIPO_WCON
	ret
	.cfi_endproc

////////////////////////////////////////////////////////////////////////////////
//                              UTILITY FUNCTIONS                             //
////////////////////////////////////////////////////////////////////////////////

#-------------------------------------------------------------------------------
# Inizio dell' ingresso da una interfaccia seriale
.GLOBAL go_inputse
#-------------------------------------------------------------------------------
go_inputse:
	movw %di, %dx		// ind. di IER in edx
	inb %dx, %al
	orb $0x01, %al		// abilitazione dell' interfaccia a
				//  generare interruzioni in ingresso
	outb %al, %dx
	ret

#-------------------------------------------------------------------------------
# Fine dell' ingresso da un' interfaccia seriale
.GLOBAL halt_inputse
#-------------------------------------------------------------------------------
halt_inputse:
	movw %di, %dx		// ind. di IER in edx
	inb %dx, %al
	and $0xfe, %al
	outb %al, %dx		// disabilitazione della generazione
				//  di interruzioni
	ret

#-------------------------------------------------------------------------------
# Inizio dell' uscita su interfaccia seriale
.GLOBAL go_outputse
#-------------------------------------------------------------------------------
go_outputse:
	movw %di, %dx		// ind. di IER in edx
	inb %dx, %al
	orb $0x02, %al
	outb %al, %dx
	ret

#-------------------------------------------------------------------------------
# Fine dell' uscita su interfaccia seriale
.GLOBAL halt_outputse
#-------------------------------------------------------------------------------
halt_outputse:
	movw %di, %dx		// ind. di IER in edx
	inb %dx, %al
	and $0xfd, %al
	outb %al, %dx
	ret

////////////////////////////////////////////////////////////////////////////////
//                         I/O SERIAL INTERFACES                              //
////////////////////////////////////////////////////////////////////////////////

.set  LCR1, 0x03fb
.set  LCR2, 0x02fb
.set  DLR_LSB1, 0x03f8
.set  DLR_LSB2, 0x02f8
.set  DLR_MSB1, 0x03f9
.set  DLR_MSB2, 0x02f9
.set  IER1,  0x03f9
.set  IER2,  0x02f9
.set  RBR1,  0x03f8
.set  RBR2,  0x02f8
.set  MCR1,  0x03fc
.set  MCR2,  0x02fc


// Inizializzazione delle interfacce seriali
.GLOBAL com_setup
com_setup:
	movb $0x80, %al
	movw $LCR1, %dx
	outb %al, %dx
	movw $0x000c, %ax
	movw $DLR_LSB1, %dx
	outb %al, %dx
	movb %ah, %al
	movw $DLR_MSB1, %dx
	outb %al, %dx
	movb $0x03, %al
	movw $LCR1, %dx
	outb %al, %dx
	movb $0x00, %al
	movw $IER1, %dx
	outb %al, %dx
	movw $RBR1, %dx
	movw $MCR1, %dx			// abilitazione porta 3-state
	movb $0b00001000, %al
	outb %al, %dx
	inb %dx, %al

	movb $0x80, %al
	movw $LCR2, %dx
	outb %al, %dx
	movw $0x000c, %ax
	movw $DLR_LSB2, %dx
	outb %al, %dx
	movb %ah, %al
	movw $DLR_MSB2, %dx
	outb %al, %dx
	movb $0x03, %al
	movw $LCR2, %dx
	outb %al, %dx
	movb $0x00, %al
	movw $IER2, %dx
	outb %al, %dx
	movw $RBR2, %dx
	movw $MCR2, %dx
	movb $0b00001000, %al
	outb %al, %dx

	inb %dx, %al
	ret

.set KBD_WCB,	0x60
.set KBD_RCB,	0x20

.macro wait_kbd_out
1:	inb $0x64, %al	  // leggi stato
	testb $0x02, %al  // busy?
	jnz 1b
.endm

	.GLOBAL go_inputkbd
go_inputkbd:
	pushq %rbp
	movq %rsp, %rbp

	movw 6(%rdi), %dx
	movb $KBD_WCB, %al
	outb %al, %dx
	movw 2(%rdi), %dx
	movb $0x61, %al
	outb %al, %dx
	movw 6(%rdi), %dx
1:	inb %dx, %al
	testb $0x02, %al
	jnz 1b

	leave
	ret


	ret

	.GLOBAL halt_inputkbd
halt_inputkbd:
	pushq %rbp
	movq %rsp, %rbp

	movw 6(%rdi), %dx
	movb $KBD_WCB, %al
	outb %al, %dx
	movw 2(%rdi), %dx
	movb $0x60, %al
	outb %al, %dx
	movw 6(%rdi), %dx
1:	inb %dx, %al
	testb $0x02, %al
	jnz 1b

	leave
	ret

// indirizzi delle porte relative alla gestione del cursore, nel controllore
// della scheda video
.set CUR_HIGH, 0x0e
.set CUR_LOW, 0x0f
.set DAT, 0x03d5


// visualizza il cursore nella posizione passata come parametro
	.GLOBAL cursore
cursore:
	pushq %rbp
	movq %rsp, %rbp
	movb  $80, %al
	mulb  %cl
	addb  %dl, %al
	adcb  $0, %ah
	movw  %ax, %cx
	movw  %di, %dx
	movb  $CUR_HIGH, %al
	outb  %al, %dx
	movw  %si, %dx
	movb  %ch, %al
	outb  %al, %dx
	movw  %di, %dx
	movb  $CUR_LOW, %al
	outb  %al, %dx
	movw  %si, %dx
	movb  %cl, %al
	outb  %al, %dx
	leave
	retq

#-------------------------------------------------------------------------------
# IDT I/O Primitive Gates initialization
.GLOBAL fill_io_gates
#-------------------------------------------------------------------------------
fill_io_gates:
    pushq %rbp
    movq %rsp, %rbp

# call fill_io_gate for each I/O primitive
    fill_io_gate    IO_TIPO_RSEN     a_readse_n
    fill_io_gate    IO_TIPO_RSELN    a_readse_ln
    fill_io_gate    IO_TIPO_WSEN     a_writese_n
    fill_io_gate    IO_TIPO_WSE0     a_writese_0
    fill_io_gate    IO_TIPO_RCON     a_readconsole
    fill_io_gate    IO_TIPO_WCON     a_writeconsole
    fill_io_gate    IO_TIPO_INIC     a_iniconsole
    fill_io_gate    IO_TIPO_HDR      a_readhd_n
    fill_io_gate    IO_TIPO_HDW      a_writehd_n
    fill_io_gate    IO_TIPO_DMAHDR   a_dmareadhd_n
    fill_io_gate    IO_TIPO_DMAHDW   a_dmawritehd_n

    leave
    ret

////////////////////////////////////////////////////////////////////////////////
//                                 I/O PRIMITIVES                             //
////////////////////////////////////////////////////////////////////////////////

	.EXTERN c_readse_n
a_readse_n:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rsi
	trojan_horse2 %rsi %rdx
	trojan_horse %rcx
	call c_readse_n
	iretq
	.cfi_endproc

	.EXTERN c_readse_ln
a_readse_ln:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rsi
	trojan_horse %rdx
	trojan_horse2 %rdx $4
	trojan_horse2 %rsi (%rdx)
	trojan_horse %rcx
	call c_readse_ln
	iretq
	.cfi_endproc

	.EXTERN c_writese_n
a_writese_n:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rdi
	trojan_horse2 %rdi %rdx
	call c_writese_n
	iretq
	.cfi_endproc

	.EXTERN c_writese_0	// non c_writese_ln, che va lo stesso
a_writese_0:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rsi
	trojan_horse %rdx
	trojan_horse2 %rdx $4
	movslq (%rdx), %r8
	trojan_horse2 %rsi %r8
	call c_writese_0
	iretq
	.cfi_endproc

	.EXTERN c_readconsole
a_readconsole:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rdi
	trojan_horse %rsi
	trojan_horse2 %rsi $4
	movslq (%rsi), %r8
	trojan_horse2 %rdi %r8
	call c_readconsole
	iretq
	.cfi_endproc

	.EXTERN c_writeconsole
a_writeconsole:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	trojan_horse %rdi
	call c_writeconsole
	iretq
	.cfi_endproc

	.EXTERN c_iniconsole
a_iniconsole:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call c_iniconsole
	iretq
	.cfi_endproc


# interface ATA

.GLOBAL			hd_write_address
hd_write_address:
	pushq %rbp
	movq %rsp, %rbp

	movl %esi,%eax
	movw 6(%rdi),%dx
	outb %al,%dx		// caricato SNR
	movw 2(%rdi),%dx
	movb %ah,%al
	outb %al,%dx		// caricato CNL
	shrl $16,%eax
	movw 4(%rdi),%dx
	outb %al,%dx		// caricato CNH
	movw 8(%rdi),%dx
	inb %dx,%al		// HND in %al
	andb $0xf0,%al		// maschera per l'indirizzo in HND
	andb $0x0f,%ah		// maschera per i 4 bit +sign di primo
	orb  $0xe0,%ah		// seleziona LBA
	orb %ah,%al
	outb %al,%dx		// caricato HND

	leave
	ret

#-------------------------------------------------------------------------------
.GLOBAL			hd_write_command
#-------------------------------------------------------------------------------
hd_write_command:
	pushq %rbp
	movq %rsp, %rbp

	movl %edi, %eax
	movw %si, %dx
	outb %al, %dx

	leave
	ret

#-------------------------------------------------------------------------------
.GLOBAL		hd_go_inout
#-------------------------------------------------------------------------------
hd_go_inout:		#...
	movw %di, %dx		// ind. di DEV_CTL in edx
	movb $0x08,%al
	outb %al, %dx			// abilitazione dell' interfaccia a
					// generare interruzioni
	ret

#-------------------------------------------------------------------------------
.GLOBAL			hd_halt_inout
#-------------------------------------------------------------------------------
hd_halt_inout:
	movw %di, %dx		// ind. di DEV_CTL in edx
	movb $0x0A,%al
	outb %al, %dx			// disabilitazione della generazione
					// di interruzioni
	ret

// Seleziona uno dei due drive di un canale ATA
	.GLOBAL hd_select_device
hd_select_device:
	pushq %rbp
	movq %rsp, %rbp

	movl %edi,%eax
	cmpl $0,%eax
	je shd_ms
shd_sl:	movb $0xf0,%al
	jmp ms_out
shd_ms:	movb $0xe0,%al
ms_out:	movw %si,%dx
	outb %al,%dx

	leave
	ret

#-------------------------------------------------------------------------------
.GLOBAL		readhd_n
#-------------------------------------------------------------------------------
readhd_n:
		.cfi_startproc
		int		$IO_TIPO_HDR
		ret
		.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL		writehd_n
#-------------------------------------------------------------------------------
writehd_n:	
		.cfi_startproc
		int		$IO_TIPO_HDW
		ret
		.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL		dmareadhd_n
#-------------------------------------------------------------------------------
dmareadhd_n:	
		.cfi_startproc
		int		$IO_TIPO_DMAHDR
		ret
		.cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL		dmawritehd_n
dmawritehd_n:
    .cfi_startproc
    int $IO_TIPO_DMAHDW
    ret
    .cfi_endproc

.EXTERN		c_readhd_n
a_readhd_n:	# routine INT $io_tipo_hdr
		.cfi_startproc
		.cfi_def_cfa_offset 40
		.cfi_offset rip, -40
		.cfi_offset rsp, -16
		trojan_horse %rdi
		trojan_horse3 %rdi %rdx
		trojan_horse %rcx
		call	c_readhd_n
		iretq
		.cfi_endproc

.EXTERN		c_writehd_n
a_writehd_n:	# routine INT $io_tipo_hdw
		.cfi_startproc
		.cfi_def_cfa_offset 40
		.cfi_offset rip, -40
		.cfi_offset rsp, -16
		trojan_horse %rdi
		trojan_horse3 %rdi %rdx
		trojan_horse %rcx
		call	c_writehd_n
		iretq
		.cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_dmareadhd_n
#-------------------------------------------------------------------------------
a_dmareadhd_n:                      # routine INT $dma_tipob_r
		.cfi_startproc
		.cfi_def_cfa_offset 40
		.cfi_offset rip, -40
		.cfi_offset rsp, -16
		trojan_horse %rdi
		trojan_horse3 %rdi %rdx
		trojan_horse %rcx
		call	c_dmareadhd_n
		iretq
		.cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_dmawritehd_n
#-------------------------------------------------------------------------------
a_dmawritehd_n:                     # routine INT $dma_tipob_w
		.cfi_startproc
		.cfi_def_cfa_offset 40
		.cfi_offset rip, -40
		.cfi_offset rsp, -16
		trojan_horse %rdi
		trojan_horse3 %rdi %rdx
		trojan_horse %rcx
		call	c_dmawritehd_n
		iretq
		.cfi_endproc
#*******************************************************************************

