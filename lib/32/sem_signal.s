################################################################################
# File: sem_signal.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
.text
.global sem_signal
sem_signal:	pushl %eax
		movl 8(%esp), %eax
		movb $1, sem(%eax)
		popl %eax
		ret
