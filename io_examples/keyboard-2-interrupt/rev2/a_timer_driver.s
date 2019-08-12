#*******************************************************************************
# File: a_timer_driver.s
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
.EXTERN c_timer_driver
#-------------------------------------------------------------------------------
.GLOBAL a_timer_driver
#-------------------------------------------------------------------------------
a_timer_driver:
    save_registers              # save general purpose registers content
    call  c_timer_driver     # call c driver implementation
    restore_registers           # restore general purpose registers content
    
    iretq                       # return from interrupt
#*******************************************************************************

