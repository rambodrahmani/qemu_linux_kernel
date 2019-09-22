# [...]

# EXTENSION 2019-07-24

    #---------------------------------------------------------------------------
    # Decrease breakpoint IDT entry DPL to USER_LEVEL in order for it to be
    # called from the User Module processes.
    #---------------------------------------------------------------------------
    carica_gate	3       breakpoint  LIV_UTENTE

# EXTENSION 2019-07-24

# [...]

# EXTENSION 2019-07-24

    # initialize bpattach IDT gate subroutine
    carica_gate  TIPO_BPA  a_bpattach  LIV_UTENTE

    # initialize bpwait IDT gate subroutine
    carica_gate  TIPO_BPW  a_bpwait    LIV_UTENTE

# EXTENSION 2019-07-24

# SOLUTION 2019-07-24

    # initialize bpdetach IDT gate subroutine
    carica_gate  TIPO_BPD  a_bpdetach  LIV_UTENTE

# SOLUTION 2019-07-24

# [...]

# EXTENSION 2019-07-24

#-------------------------------------------------------------------------------
# IDT gates subroutines assembly deiniftions.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpattach:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato                    # save current process state
    call c_bpattach                     # call C++ subroutine implementation
    call carica_stato                   # load new process state
    iretq                               # return from interrupt
    .cfi_endproc

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpwait:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato                    # save current process state
    call c_bpwait                       # call C++ subroutine implementation
    call carica_stato                   # load new process state
    iretq                               # return from interrupt
    .cfi_endproc

# EXTENSION 2019-07-24

# SOLUTION 2019-07-24

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpdetach:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato                    # save current process state
    call c_bpdetach                     # call C++ subroutine implementation
    call carica_stato                   # load new process state
    iretq                               # return from interrupt
    .cfi_endproc

# SOLUTION 2019-07-24

# [...]

#-------------------------------------------------------------------------------
# Interrupt 3 - Breakpoint exception.
# Sets the parameters for and calls the C++ exception handler. A new process is
# scheduled in the C++ implementation.
#-------------------------------------------------------------------------------
breakpoint:
	.cfi_startproc
	.cfi_def_cfa_offset 40
	.cfi_offset rip, -40
	.cfi_offset rsp, -16
	call salva_stato                # save current process state

# SOLUTION 2019-07-24

    movq $3, %rdi                   # exception type
    movq $0, %rsi                   # exception error
    movq %rsp, %rdx                 # %address contained in rsp
    call c_breakpoint_exception     # call C++ exception handler

# SOLUTION 2019-07-24

	call carica_stato               # load new process state
	iretq                           # return from interrupt.
	.cfi_endproc

# [...]

