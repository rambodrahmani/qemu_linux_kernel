#*******************************************************************************
# File: a_driver.s
#       Revision 4. One last problem arises from how the registers will be used
#       by c_driver(). The compiler won't save the content of registers rsi, rdi
#       etc... because c_driver() is considered to be just a C++ function.
#       We now that c_drver() can be executed anytime in between two
#       instructions of the main program. If this happens, the main program
#       might end up with the wrong values in some of the registers.
#       In order to fix this, we use two library functions which will save the
#       content of all registers before calling c_driver and restore them all
#       back after c_driver is done.
#
#       Interrupt primitive 40.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 11/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "../../../lib/libqlk.s"
#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.EXTERN c_driver
#-------------------------------------------------------------------------------
.GLOBAL a_driver
#-------------------------------------------------------------------------------
a_driver:
    save_registers              # save general purpose registers content
    call  c_driver              # call c driver implementation
    restore_registers           # restore general purpose registers content
    
    iretq                       # return from interrupt
#*******************************************************************************

