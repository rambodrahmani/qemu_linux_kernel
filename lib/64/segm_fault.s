#include "def.h"
.global segm_fault
segm_fault:
	movq $11, %rdi
	popq %rsi
	popq %rdx
	jmp comm_exc

