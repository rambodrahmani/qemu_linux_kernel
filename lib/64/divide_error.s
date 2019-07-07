#include "def.h"
.global divide_error

divide_error:
	movq $0, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

