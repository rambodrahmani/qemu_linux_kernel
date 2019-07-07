#include "def.h"
.global unknown_exc
unknown_exc:
	movq $15, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

