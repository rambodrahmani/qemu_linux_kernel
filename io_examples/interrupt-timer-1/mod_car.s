#*******************************************************************************
# File: mod_car.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 18/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_go_timer
#-------------------------------------------------------------------------------
.GLOBAL a_go_timer   # interrupt primitive int 242
#-------------------------------------------------------------------------------
a_go_timer:
    call    c_go_timer	
    iretq

#-------------------------------------------------------------------------------
.EXTERN c_driver_timer
#-------------------------------------------------------------------------------
.GLOBAL a_driver_timer  # interrupt driver int 42
#-------------------------------------------------------------------------------
a_driver_timer:
    save_registers              # save general purpose registers content

    call     c_driver_timer     # call C++ driver body
    movabsq  $0xFEE000B0, %rax  # send EOI
    movl     $0, (%rax)         # leave return result

    restore_registers           # restore general purpose registers content
    iretq                       # return from interrupt
#*******************************************************************************

