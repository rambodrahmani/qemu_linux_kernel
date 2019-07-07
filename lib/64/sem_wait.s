.text
.global sem_wait
sem_wait:	
		movb $0, sem(%edi)
		sti
aspetta:	testb $1, sem(%edi)
		jz aspetta
		ret
