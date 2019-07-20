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
.EXTERN     c_leggi_linea
.EXTERN     c_driv_tasint
#-------------------------------------------------------------------------------
.GLOBAL     a_leggi_linea
#-------------------------------------------------------------------------------
a_leggi_linea:                  # INT $230 subroutine
    call    c_leggi_linea
    iretq

#-------------------------------------------------------------------------------
.GLOBAL     a_driv_tasint
#-------------------------------------------------------------------------------
a_driv_tasint:                  # INT $30 subroutine
    save_registers              # push all registers

    call     c_driv_tasint      # call driver
    movabsq  $0xFEE000B0, %rax  # write End Of Interrupt
    movl     $0, (%rax)

    restore_registers           # pop all registers
    iretq
#*******************************************************************************

