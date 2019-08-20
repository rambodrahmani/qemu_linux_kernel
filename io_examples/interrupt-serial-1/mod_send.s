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
.EXTERN c_serial_send
#-------------------------------------------------------------------------------
.GLOBAL a_serial_send       # interrupt 244 primitive
#-------------------------------------------------------------------------------
a_serial_send:
    call   c_serial_send    # call C++ implementation for the primitive
    iretq

#-------------------------------------------------------------------------------
.EXTERN c_serial_driverout
#-------------------------------------------------------------------------------
.GLOBAL a_serial_driverout       # interrupt 44 primitive
#-------------------------------------------------------------------------------
a_serial_driverout:
    save_registers               # save general purpose registers content

    call     c_driverout_serial  # call C++ driver implementation
    movabsq  $0xFEE000B0, %rax   # send EOI
    movl     $0, (%rax)          # return result

    restore_registers            # restore general purpose registers content
    iretq                        # return from interrupt
#*******************************************************************************

