#*******************************************************************************
# File: page_fault.s
#       Page fault.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
#include "def.h"
#-------------------------------------------------------------------------------
.GLOBAL page_fault
#-------------------------------------------------------------------------------
page_fault:
    movq  $14, %rdi
    popq  %rsi
    popq  %rdx
    jmp   comm_exc
#*******************************************************************************

#*******************************************************************************
# Interrupt 14 -- Page Fault
# This exception occurs when paging is enabled (PG=1) and the processor detects
# one of the following conditions while translating a linear address to a
# physical address:
#
# 1. The page-directory or page-table entry needed for the address translation
#    has zero in its present bit;
# 2. The current procedure does not have sufficient privilege to access the
#    indicated page.
#*******************************************************************************

