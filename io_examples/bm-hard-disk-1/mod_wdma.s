#*******************************************************************************
# File: mod_send.s
#       Assembly implementation for primitives 244 and 44.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN  c_go_wdma                       # primitive 251 C++ implementation 
#-------------------------------------------------------------------------------
.GLOBAL  a_go_wdma                       # primitive 251 assembly implementation
#-------------------------------------------------------------------------------
a_go_wdma: 
    call   c_go_wdma
    iretq

#-------------------------------------------------------------------------------
.EXTERN  c_driver_wdma                    # primitive 51 C++ implementation
#-------------------------------------------------------------------------------
.GLOBAL  a_driver_wdma                    # primitive 51 assembly implementation
#-------------------------------------------------------------------------------
a_driver_wdma:
    save_registers                  # save general purpose registers content

    call     c_driver_wdma          # call driver C++ body implementation
    movabsq  $0xFEE000B0, %rax      # send EOI
    movl     $0, (%rax)

    restore_registers               # restore general purpose registers content
    iretq                           # return from interrupt
#*******************************************************************************

