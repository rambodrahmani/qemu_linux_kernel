# SOLUTION 2016-07-06

    fill_io_gate    IO_TIPO_CEREAD  a_ceread

# SOLUTION 2016-07-06

# SOLUTION 2016-07-06

#-------------------------------------------------------------------------------
.EXTERN c_ceread                               # C++ implementation for a_ceread
#-------------------------------------------------------------------------------
# IDT subroutine for the IO_TIPO_CEREAD interrupt.
a_ceread:
    cavallo_di_troia %rsi           # check buffer address
    cavallo_di_troia %rdx           # check quanti address
    cavallo_di_troia2 %rdx $4       # quanti will contain a integer (4 bytes)
    cavallo_di_troia2 %rsi (%rdx)   # check the buffer entire length
    call c_ceread                   # call C++ int. primitive implementation
    iretq                           # return from interrupt

# SOLUTION 2016-07-06

