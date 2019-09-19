# SOLUTION 2016-07-27

    # set interrupt handler
    fill_io_gate	IO_TIPO_CEDMAREAD	a_cedmaread

# SOLUTION 2016-07-27

# SOLUTION 2016-07-27

#-------------------------------------------------------------------------------
.EXTERN c_cedmaread                        # C++ implementation for a_cedmaread
#-------------------------------------------------------------------------------

a_cedmaread:                               # IO_TIPO_CEDMAREAD interrupt handler
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    cavallo_di_troia %rsi            # check buffer length reference address
    cavallo_di_troia2 %rsi $4        # 'quanti' need 4 bytes
    cavallo_di_troia %rdx            # check destination buffer starting address
    movl (%rsi), %r9d                # move destination buffer length to %r9d
    cavallo_di_troia2 %rdx %r9       # check destination buffer length
    call c_cedmaread                 # call C++ interrupt handler implementation
    iretq                            # return from interrupt
    .cfi_endproc

# SOLUTION 2016-07-27

