.text

.extern c_delay
.global a_delay              # dichiarazione necessaria per la funzione ini()
a_delay:             # primitiva INT $240
	call c_delay
	iret
