simd_exc:
.global simd_exc
	movq $19, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

