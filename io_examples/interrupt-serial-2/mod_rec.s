#*******************************************************************************
# File: mod_rec.s
#       Assembly implementation for primitives 245 and 45.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_serial_receive
#-------------------------------------------------------------------------------
.GLOBAL a_serial_receive        # interrupt 245 primitive
#-------------------------------------------------------------------------------
a_serial_receive:
    call   c_serial_receive
    iretq

#-------------------------------------------------------------------------------
.EXTERN c_serial_driverin
#-------------------------------------------------------------------------------
.GLOBAL a_serial_driverin       # interrupt 45 primitive 
#-------------------------------------------------------------------------------
a_serial_driverin:
    save_registers              # save general purpose registers content

    call     c_serial_driverin  # call C++ driver implementation
    movabsq  $0xFEE000B0, %rax  # send EOI
    movl     $0, (%rax)         # return result

    restore_registers           # restore general purpose registers content
    iretq                       # return from interrupt
#*******************************************************************************

