# [...]

# EXTENSION 2019-06-12

#-------------------------------------------------------------------------------
.GLOBAL breakpoint                       # extern "C" natl breakpoint(vaddr rip)
#-------------------------------------------------------------------------------
breakpoint:
    .cfi_startproc
    int $TIPO_B
    ret
    .cfi_endproc

# EXTENSION 2019-06-12

