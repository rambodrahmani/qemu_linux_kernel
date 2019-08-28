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
.EXTERN  c_go_rdma
#-------------------------------------------------------------------------------
.GLOBAL  a_go_rdma                                # primitive 252 implementation
#-------------------------------------------------------------------------------
a_go_rdma: 
    call   c_go_rdma
    iretq

#-------------------------------------------------------------------------------
.EXTERN  c_driver_rdma
#-------------------------------------------------------------------------------
.GLOBAL  a_driver_rdma                             # primitive 52 implementation
#-------------------------------------------------------------------------------
a_driver_rdma:
    save_registers              # save general purpose registers content

    call    c_driver_rdma       # call driver C++ body implementation
    movabsq	$0xFEE000B0, %rax	# send EOI
    movl    $0, (%rax)

    restore_registers           # restore general purpose registers content
    iretq
#*******************************************************************************

