#include "def.h"
.global stack_fault
stack_fault:
	movq $12, %rdi
	popq %rsi
	popq %rdx
	jmp comm_exc

