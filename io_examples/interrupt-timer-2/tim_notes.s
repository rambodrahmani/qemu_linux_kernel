#*******************************************************************************
# File: tim_notes.s
#       Primitive note() Assembly implementation.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 18/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
#-------------------------------------------------------------------------------
.GLOBAL note
#-------------------------------------------------------------------------------
note:
    int  $243
    ret
#*******************************************************************************

