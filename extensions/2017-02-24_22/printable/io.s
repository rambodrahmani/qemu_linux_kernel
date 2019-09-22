
# EXTENSION 2017-02-24

#-------------------------------------------------------------------------------
.global resident
#-------------------------------------------------------------------------------
resident:
    nt $TIPO_RES
    ret

#-------------------------------------------------------------------------------
.global nonresident
#-------------------------------------------------------------------------------
nonresident:
    int $TIPO_NONRES
    ret

#-------------------------------------------------------------------------------
.global readCR3                # returns che content of the CR3 register in %eax
#-------------------------------------------------------------------------------
readCR3:
    movq %cr3, %rax
    retq

# EXTENSION 2017-02-24


# SOLUTION 2017-02-24

    # fill IDT gate for IO_TIPO_CEDMAREAD interrupts
    fill_io_gate    IO_TIPO_CEDMAREAD   a_cedmaread

# SOLUTION 2017-02-24

# SOLUTION 2017-02-24

#-------------------------------------------------------------------------------
.EXTERN c_cedmaread                                         # C++ implementation
#-------------------------------------------------------------------------------
a_cedmaread:
    .cfi_startproc
    .cfi_def_cfa_offset 40
    .cfi_offset rip, -40
    .cfi_offset rsp, -16
    cavallo_di_troia %rdx           # check destination buffer starting address
    cavallo_di_troia2 %rdx %rsi     # check destination buffer length
    call c_cedmaread                # call C++ implementation
    iretq                           # return from interrupt
    .cfi_endproc

# SOLUTION 2017-02-24

