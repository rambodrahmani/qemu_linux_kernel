#include "def.h"
.global overflow
overflow:
	movq $4, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

