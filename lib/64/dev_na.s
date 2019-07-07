#include "def.h"
.global dev_na
dev_na:
	movq $7, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

