#*******************************************************************************
# File: keyboard_init.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 19/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
.EXTERN     c_read_line
.EXTERN     c_keyboard_driver
#-------------------------------------------------------------------------------
.GLOBAL     a_read_line
#-------------------------------------------------------------------------------
a_read_line:                    # INT $230 subroutine
    call    c_read_line
    iretq

#-------------------------------------------------------------------------------
.GLOBAL     a_keyboard_driver
#-------------------------------------------------------------------------------
a_keyboard_driver:              # INT $30 subroutine
    save_registers              # push all registers

    call     c_keyboard_driver  # call driver
    movabsq  $0xFEE000B0, %rax  # write End Of Interrupt
    movl     $0, (%rax)

    restore_registers           # pop all registers
    iretq
#*******************************************************************************

