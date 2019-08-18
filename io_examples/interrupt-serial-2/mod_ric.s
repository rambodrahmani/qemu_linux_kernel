#include "libce.s"
#****************************************************************************************************
# file mod_ric.s

.text

.extern		c_ricevi_serial
.global 	a_ricevi_serial			# dichiarazione necessaria per la funzione ini()
a_ricevi_serial:				# routine INT $242
		call 	c_ricevi_serial
		iretq

.extern		c_driverin_serial
.global 	a_driverin_serial		# dichiarazione necessaria per la funzione ini()
a_driverin_serial:				# routine associata al tipo 42
		salva_registri
		call	c_driverin_serial
		movabsq	$0xFEE000B0, %rax		# invio di EOI
		movl $0, (%rax)
		carica_registri
		iretq
#****************************************************************************************************
