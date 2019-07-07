#include "def.h"
.global debug
debug:
	movq $1, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

