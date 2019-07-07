.text
.global sem_wait
sem_wait:	pushl %eax
		movl 8(%esp), %eax
		movb $0, sem(%eax)
		sti
aspetta:	testb $1, sem(%eax)
		jz aspetta
		popl %eax
		ret
