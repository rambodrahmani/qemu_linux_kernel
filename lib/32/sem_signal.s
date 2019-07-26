#*******************************************************************************
# File: sem_signal.s
#       Sets the specified semaphore to 1.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL sem_signal
#-------------------------------------------------------------------------------
sem_signal:
    pushl %eax
    movl  8(%esp), %eax     # retrieve semaphore index
    movb  $1, sem(%eax)     # set the sempahore to 1
    popl  %eax
    ret
#*******************************************************************************

