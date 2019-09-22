# [...]

# EXTENSION 2019-07-24

#-------------------------------------------------------------------------------
.GLOBAL bpattach
#-------------------------------------------------------------------------------
#                Implementation for extern "C" bool bpattach(natl id, vaddr rip)
#-------------------------------------------------------------------------------
bpattach:
    .cfi_startproc
    int $TIPO_BPA
    ret
    .cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL bpwait                     # Implementation for extern "C" void bpwait()
#-------------------------------------------------------------------------------
bpwait:
    .cfi_startproc
    int $TIPO_BPW
    ret
    .cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL bpdetach                 # Implementation for extern "C" void bpdetach()
#-------------------------------------------------------------------------------
bpdetach:
    .cfi_startproc
    int $TIPO_BPD
    ret
    .cfi_endproc

# EXTENSION 2019-07-24

