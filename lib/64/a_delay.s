#*******************************************************************************
# File: a_delay.s
#       Interrupt primitive 240.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_delay
#-------------------------------------------------------------------------------
.GLOBAL a_delay
#-------------------------------------------------------------------------------
a_delay:
    call c_delay
    iretq
#*******************************************************************************

