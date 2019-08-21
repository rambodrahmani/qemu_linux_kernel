#include "libce.s"
#****************************************************************************************************
# file mod_hddw.s

.text

.extern 		c_go_hddw
.global 		a_go_hddw			# dichiarazione necessaria per la funzione ini()
a_go_hddw:						# routine INT $245
			call 	c_go_hddw
			iretq

.extern		c_driver_hddw
.global 		a_driver_hddw			# dichiarazione necessaria per la funzione ini()
a_driver_hddw:						# routine associata al tipo 45
			salva_registri
			call 	c_driver_hddw
			movabsq	$0xFEE000B0, %rax	# invio di EOI
			movl $0, (%rax)
			carica_registri
			iretq
#****************************************************************************************************
