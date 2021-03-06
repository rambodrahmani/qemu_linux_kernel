#*******************************************************************************
# File: a_driver.s
#       Revision 3.
#       Interrupt primitive 40.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 11/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_driver
#-------------------------------------------------------------------------------
.GLOBAL a_driver
#-------------------------------------------------------------------------------
a_driver:
    call  c_driver              # call c driver implementation
    iretq                       # return from interrupt
#*******************************************************************************

