#include "def.h"
.global prot_fault
prot_fault:
	movq $13, %rdi
	popq %rsi
	popq %rdx
	jmp comm_exc

