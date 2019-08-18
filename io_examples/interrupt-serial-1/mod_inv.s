#include "libce.s"
#****************************************************************************************************
# file mod_inv.s

.text

.extern		c_invia_serial
.global 	a_invia_serial			# dichiarazione necessaria per la funzione ini()
a_invia_serial:					# routine INT $243
		call 		c_invia_serial
		iretq

.extern		c_driverout_serial
.global 	a_driverout_serial		# dichiarazione necessaria per la funzione ini()
a_driverout_serial:				# routine associata al tipo 43
		salva_registri
		call	c_driverout_serial
		movabsq	$0xFEE000B0, %rax		# invio di EOI
		movl $0, (%rax)
		carica_registri
		iretq
#****************************************************************************************************
