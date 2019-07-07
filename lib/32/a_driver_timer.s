.text

.extern c_driver_timer
.global a_driver_timer       # dichiarazione necessaria per la funzione ini()
a_driver_timer:      	     # driver di interruzione, associato al tipo 40
	pushal
	call c_driver_timer
	movl $0, 0xFEE000B0  # invio di End Of Interrupt
	popal
	iret
