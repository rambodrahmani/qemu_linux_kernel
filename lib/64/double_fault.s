#include "def.h"
.global double_fault
double_fault:
	movq $8, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

