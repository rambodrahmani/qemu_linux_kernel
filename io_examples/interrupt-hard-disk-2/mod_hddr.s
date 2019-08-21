#include "libce.s"
#****************************************************************************************************
# file mod_hddr.s

.text

.extern 		c_go_hddr
.global 		a_go_hddr			# dichiarazione necessaria per la funzione ini()
a_go_hddr:						# routine INT $244
			call 	c_go_hddr
			iretq

.extern		c_driver_hddr
.global 		a_driver_hddr			# dichiarazione necessaria per la funzione ini()
a_driver_hddr:						# routine associata al tipo 44
			salva_registri
			call 	c_driver_hddr
			movabsq	$0xFEE000B0, %rax		# invio di EOI
			movl 	$0, (%rax)
			carica_registri
			iretq
#****************************************************************************************************
