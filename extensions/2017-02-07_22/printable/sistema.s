# [...]

#-------------------------------------------------------------------------------
# Initialize IDT gates for the user primitives. This gates must have user level
# GLs as the User module must be able to use theese interrupts from the provided
# primitives.
#-------------------------------------------------------------------------------

# EXTENSION 2017-02-07

    # natq countres();
    carica_gate  TIPO_CRES    a_countres     LIV_UTENTE

    # void nonresident(natl id);
    carica_gate  TIPO_NONRES  a_nonresident  LIV_UTENTE

# EXTENSION 2017-02-07 )

# SOLUTION 2017-02-07

    # natl resident(addr base, natq size);
    carica_gate TIPO_RES      a_resident     LIV_UTENTE

# SOLUTION 2017-02-07 )

# [...]

# EXTENSION 2017-02-07

#-------------------------------------------------------------------------------
# IDT Subroutines definitions.
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
.EXTERN c_countres                                            # natq countres();
#-------------------------------------------------------------------------------
a_countres:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call c_countres                 # call C++ implementation
    iretq                           # return from interrupt
    .cfi_endproc

#-------------------------------------------------------------------------------
.EXTERN c_nonresident                               # void nonresident(natl id);
#-------------------------------------------------------------------------------
a_nonresident:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato                # save current process state
    call c_nonresident              # call C++ implementation
    call carica_stato               # load new process state
    iretq                           # return from interrupt
    .cfi_endproc

# EXTENSION 2017-02-07

# SOLUTION 2017-02-07

#-------------------------------------------------------------------------------
.extern c_resident                        # natl resident(addr base, natq size);
#-------------------------------------------------------------------------------
a_resident:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    call salva_stato                # save current process state
    cavallo_di_troia %rdi           # check addressed pages base
    cavallo_di_troia2 %rdi %rsi     # check addressed pages length
    call c_resident                 # call C++ implementation
    call carica_stato               # load new process state
    iretq                           # return from interrupt
    .cfi_endproc

# SOLUTION 2017-02-07 )

# [...]

