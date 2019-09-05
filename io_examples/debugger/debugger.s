#*******************************************************************************
# File: debugger.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 05/09/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
.EXTERN c_debug
.GLOBAL a_debug
#-------------------------------------------------------------------------------
a_debug:
    save_registers                  # save general purpose registers content
    movq 120(%rsp), %rdi            # argument for foo()
    call c_debug                    # call primitive C++ body
    restore_registers               # restore general purpose registers content
    decq (%rsp)                     # decrement rsp of 1
    iretq                           # return from interrupt

