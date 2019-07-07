#include "def.h"
.global mc_exc
mc_exc:
	movq $18, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

