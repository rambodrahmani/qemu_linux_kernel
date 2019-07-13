################################################################################
# File: start.s
#       Once the user program has been loaded into memory by the boot-loader and
#       the switch between real mode to protected mode is done, the execution
#       starts from the start entry point.first of all the init_all function is
#       called. After that the user defined main function is called and once the
#       user program execution is terminated, the stop_all and tiple_fault_idt
#       subroutines are executed.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 13/07/2019.
################################################################################

#-------------------------------------------------------------------------------
.GLOBAL start, _start
.TEXT
#-------------------------------------------------------------------------------
start:
_start:
    call    init_all
    call    main
    call    stop_all
    lidt    triple_fault_idt
    int     $1

triple_fault_idt:
    .word 0
    .quad 0
#*******************************************************************************

