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
.EXTERN c_go_hddr
#-------------------------------------------------------------------------------
.GLOBAL a_go_hddr                   # primitive 247 assembly implementation
#-------------------------------------------------------------------------------
a_go_hddr:
    call   c_go_hddr
    iretq                           # return from interrupt

#-------------------------------------------------------------------------------
.EXTERN c_driver_hddr
#-------------------------------------------------------------------------------
.GLOBAL a_driver_hddr               # primitive 47 assembly implementation
#-------------------------------------------------------------------------------
a_driver_hddr:
    save_registers                  # save general purpose registers content

    call     c_driver_hddr
    movabsq  $0xFEE000B0, %rax		# send EOI
    movl     $0, (%rax)

    restore_registers               # restore general purpose registers content
    iretq                           # return from interrupt
#*******************************************************************************

