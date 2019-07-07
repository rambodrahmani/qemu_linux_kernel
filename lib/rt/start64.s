.global start,_start
.text
start:				// entry point
_start:
	call init_all
        call main
	call stop_all
	lidt triple_fault_idt
	int $1

triple_fault_idt:
	.word 0
	.quad 0
