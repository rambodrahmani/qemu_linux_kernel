################################################################################
# File: a_driver_timer.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "../libqlk.s"
.text

.extern c_driver_timer
.global a_driver_timer       # dichiarazione necessaria per la funzione ini()
a_driver_timer:      	     # driver di interruzione, associato al tipo 40
	salva_registri
	call c_driver_timer
	movq $0xFEE000B0, %rax
	movl $0, (%rax)  # invio di End Of Interrupt
	carica_registri
	iretq
