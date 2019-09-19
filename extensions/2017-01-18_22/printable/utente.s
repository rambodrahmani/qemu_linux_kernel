# [...]

# EXTENSION 2016-09-20

##
# PRIMITIVES DEFINITIONS. Each primitive will only call the corresponding
# interrupt type and return. The interrupt will be handled in the system
# module by the subroutine loaded in the IDT. This calling mechanism allows for
# the user module to be able to interact with the system module (privileges
# escalation) while maintaining isolation.
##

#-------------------------------------------------------------------------------
.GLOBAL reg                                # Primitive void reg() implementation
#-------------------------------------------------------------------------------
# Registers the calling process to the global system broadcast.
#-------------------------------------------------------------------------------
reg:
    int $TIPO_R
    ret

#-------------------------------------------------------------------------------
.GLOBAL listen                          # Primitive natl listen() implementation
#-------------------------------------------------------------------------------
# Retrieves the next broadcast message if there is any. If not the calling
# process will be placed in the global system broadcast wait queue.
#-------------------------------------------------------------------------------
listen:
    int $TIPO_LS
    ret

#-------------------------------------------------------------------------------
.GLOBAL broadcast            # Primitive void broadcast(natl msg) implementation
#-------------------------------------------------------------------------------
# Broadcasts the given message (type natl) using the system global broadcast.
#-------------------------------------------------------------------------------
broadcast:
    int $TIPO_B
    ret

# EXTENSION 2016-09-20

