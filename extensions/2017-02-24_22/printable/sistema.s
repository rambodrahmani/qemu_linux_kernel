
# EXTENSION 2017-02-24

    carica_gate  TIPO_CRES  a_countres  LIV_UTENTE

# EXTENSION 2017-02-24

	// primitive per il livello I/O
	carica_gate	TIPO_APE	a_activate_pe	LIV_SISTEMA
	carica_gate	TIPO_WFI	a_wfi		LIV_SISTEMA
	carica_gate	TIPO_FG		a_fill_gate	LIV_SISTEMA
	carica_gate	TIPO_P		a_panic		LIV_SISTEMA
	carica_gate	TIPO_AB		a_abort_p	LIV_SISTEMA
	carica_gate	TIPO_TRA	a_trasforma	LIV_SISTEMA

# EXTENSION 2017-02-24

    carica_gate  TIPO_NONRES  a_nonresident  LIV_SISTEMA
    carica_gate  TIPO_RES     a_resident     LIV_SISTEMA

# EXTENSION 2017-02-24


# EXTENSION 2017-02-24

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

# EXTENSION 2017-02-24

