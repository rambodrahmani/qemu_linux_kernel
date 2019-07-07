#include "def.h"
.global nmi
nmi:
	movq $2, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

