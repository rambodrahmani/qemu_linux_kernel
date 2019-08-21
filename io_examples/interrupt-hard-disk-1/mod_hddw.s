#*******************************************************************************
# File: mod_hddw.s
#       Assembly implementation for primitives 246 and 46.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 21/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_go_hddw
#-------------------------------------------------------------------------------
.GLOBAL a_go_hddw               # primitive 246 assembly implementation
#-------------------------------------------------------------------------------
a_go_hddw:
    call   c_go_hddw            # call primitive body C++ implementation
    iretq                       # return from interrupt

#-------------------------------------------------------------------------------
.EXTERN c_driver_hddw
#-------------------------------------------------------------------------------
.GLOBAL a_driver_hddw           # primitive 46 assembly implementation
#-------------------------------------------------------------------------------
a_driver_hddw:
    save_registers              # save general purpose registers content

    call     c_driver_hddw      # call driver body C++ implementation
    movabsq  $0xFEE000B0, %rax  # send EOI
    movl     $0, (%rax)         # return value

    restore_registers           # save general purpose registers content
    iretq                       # return from interrupt
#*******************************************************************************

