# [...]

# EXTENSION 2019-07-03

    # redefine interrupt 3 dpl level to user level in order for the User Module
    # processes to be able to use the int3 instruction
    carica_gate  3  breakpoint  LIV_UTENTE

# EXTENSION 2019-07-03

# [...]

# EXTENSION 2019-07-03

    # init IDT gate subroutine for the bpadd() primitive
    carica_gate  TIPO_BPA  a_bpadd   LIV_UTENTE

    # init IDT gate subroutine for the bpwait() primitive
    carica_gate  TIPO_BPW  a_bpwait  LIV_UTENTE

# EXTENSION 2019-07-03

# SOLUTION 2019-07-03

    # init IDT gate subroutine for the bpremove() primitive
    carica_gate  TIPO_BPR  a_bpremove  LIV_UTENTE

# SOLUTION 2019-07-03

# [...]

# EXTENSION 2019-07-03

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpadd:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    call c_bpadd
    call carica_stato
    iretq
    .cfi_endproc

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpwait:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    call c_bpwait
    call carica_stato
    iretq
    .cfi_endproc

# EXTENSION 2019-07-03

# SOLUTION 2019-07-03

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
a_bpremove:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato
    call c_bpremove
    call carica_stato
    iretq
    .cfi_endproc

# SOLUTION 2019-07-03

# [...]

#-------------------------------------------------------------------------------
# Interrupt 3 - Breakpoint exception.
# We must redefine the subroutine handling the breakpoint exception in order to
# call a custom C++ implementation which will reschedule waiting processes and
# udpate the system global breakpoint descriptor status. This assembly routine
# loads the arguments for and calls the C++ handler.
#-------------------------------------------------------------------------------
breakpoint:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato

# SOLUTION 2019-07-03

    movq $3, %rdi                   # exception type
    movq $0, %rsi                   # exception error
    movq (%rsp), %rdx               # current value addressed by %rsp
    call c_breakpoint_exception

# SOLUTION 2019-07-03

    call carica_stato
    iretq
    .cfi_endproc

# [...]

