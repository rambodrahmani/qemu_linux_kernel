################################################################################
# File: sem_wait.s
#       Sets the specified semaphore to 0 and waits for it to be set to 1.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL sem_wait
#-------------------------------------------------------------------------------
sem_wait:
    pushl  %eax
    movl   8(%esp), %eax
    movb   $0, sem(%eax)
    sti

aspetta:
    testb  $1, sem(%eax)
    jz     aspetta
    popl   %eax
    ret

