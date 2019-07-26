#*******************************************************************************
# File: commm_exc.s
#       Calls the handle_exception subroutine and halts the central processing
#       unit (CPU) until the next external interrupt is fired.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL comm_exc
#-------------------------------------------------------------------------------
comm_exc:
    call  handle_exception      # call handle_exception
    hlt                         # halt
#*******************************************************************************

