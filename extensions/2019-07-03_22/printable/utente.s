# [...]

# EXTENSION 2019-07-03

#-------------------------------------------------------------------------------
.GLOBAL bpadd             # Implementation for extern "C" bool bpadd(vaddr rip);
#-------------------------------------------------------------------------------
bpadd:
    .cfi_startproc
    int $TIPO_BPA
    ret
    .cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL bpwait                    # Implementation for extern "C" natl bpwait();
#-------------------------------------------------------------------------------
bpwait:
    .cfi_startproc
    int $TIPO_BPW
    ret
    .cfi_endproc

#-------------------------------------------------------------------------------
.GLOBAL bpremove                # Implementation for extern "C" void bpremove();
#-------------------------------------------------------------------------------
bpremove:
    .cfi_startproc
    int $TIPO_BPR
    ret
    .cfi_endproc

# EXTENSION 2019-07-03

