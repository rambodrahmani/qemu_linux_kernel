#*******************************************************************************
# File: a_keyboard_driver.s
#       Revision 1. No differences to point out.
#
#       Interrupt primitive 40.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 11/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_keyboard_driver
#-------------------------------------------------------------------------------
.GLOBAL a_keyboard_driver
#-------------------------------------------------------------------------------
a_keyboard_driver:
    save_registers              # save general purpose registers content
    call  c_keyboard_driver     # call c driver implementation
    restore_registers           # restore general purpose registers content
    
    iretq                       # return from interrupt
#*******************************************************************************

