
# EXTENSION 2017-02-24

#-------------------------------------------------------------------------------
.GLOBAL countres                                   # extern "C" natq countres();
#-------------------------------------------------------------------------------
countres:
    int $TIPO_CRES
    ret

#-------------------------------------------------------------------------------
.GLOBAL cedmaread  # extern "C" bool cedmaread(natl id, natl quanti, char *buf);
#-------------------------------------------------------------------------------
cedmaread:
    int $IO_TIPO_CEDMAREAD
    ret

# EXTENSION 2017-02-24

