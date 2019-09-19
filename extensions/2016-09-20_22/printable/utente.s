# EXTENSION 2016-09-20

##
# Definitions for the primitives declared in sys.h. They all call the
# corresponding interrupts declared in costanti.h.
##

#-------------------------------------------------------------------------------
.GLOBAL reg                                # Primitive void reg() implementation
#-------------------------------------------------------------------------------
reg:
    int $TIPO_R
    ret

#-------------------------------------------------------------------------------
.GLOBAL listen                          # Primitive natl listen() implementation
#-------------------------------------------------------------------------------
listen:
    int $TIPO_LS
    ret

#-------------------------------------------------------------------------------
.GLOBAL broadcast            # Primitive void broadcast(natl msg) implementation
#-------------------------------------------------------------------------------
broadcast:
    int $TIPO_B
    ret

# EXTENSION 2016-09-20

