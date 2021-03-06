#*******************************************************************************
# File: sem_wait.s
#       Sets the specified semaphore to 0 and waits for it to be set to 1.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 20/03/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL sem_wait
#-------------------------------------------------------------------------------
sem_wait:	
    movb  $0, sem(%edi)     # set the semaphore to 0
    sti                     # enable interrupts to avoid deadlocks

wait:
    testb  $1, sem(%edi)    # test the semaphore value
    jz     wait             # wait for the semaphore value to be set to 1
    ret
#*******************************************************************************

