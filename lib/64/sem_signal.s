.text
.global sem_signal
sem_signal:	
		movb $1, sem(%edi)
		ret
