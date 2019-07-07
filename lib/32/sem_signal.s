.text
.global sem_signal
sem_signal:	pushl %eax
		movl 8(%esp), %eax
		movb $1, sem(%eax)
		popl %eax
		ret
