#*******************************************************************************
# File: delay.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.GLOBAL delay
#-------------------------------------------------------------------------------
delay:
    int   $240
    ret
#*******************************************************************************
