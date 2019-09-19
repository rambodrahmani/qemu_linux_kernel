# SOLUTION 2016-06-15

	fill_io_gate	IO_TIPO_CEREAD	a_cedmaread

# SOLUTION 2016-06-15

# SOLUTION 2016-06-15

# cedmaread(natl id, char * buf, natl quanti)
# id -> %rdi
# &buf -> %rsi
# quanti -> %rdx
.EXTERN c_cedmaread
a_cedmaread:
    cavallo_di_troia %rsi           # check the buffer starting address
    movl %edx, %edx                 # zero out %rdx upper 32 bits
    cavallo_di_troia2 %rsi %rdx     # check the entire buffer
    call c_cedmaread                # call C++ primitive implementation
    iretq                           # return from interrupt

# SOLUTION 2016-06-15

