#include "def.h"
.global invalid_tss
invalid_tss:
	movq $10, %rdi
	popq %rsi
	popq %rdx
	jmp comm_exc

