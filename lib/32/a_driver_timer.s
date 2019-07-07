################################################################################
# File: a_driver_timer.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_driver_timer
#-------------------------------------------------------------------------------
.GLOBAL a_driver_timer       # dichiarazione necessaria per la funzione ini()
#-------------------------------------------------------------------------------
a_driver_timer:      	     # driver di interruzione, associato al tipo 40
    pushal
    call c_driver_timer
    movl $0, 0xFEE000B0  # invio di End Of Interrupt
    popal
    iret

