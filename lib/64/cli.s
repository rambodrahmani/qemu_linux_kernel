#*******************************************************************************
# File: cli.s
#       Clears interrupt flag; interrupts disabled when interrupt flag cleared.
#       If protected-mode virtual interrupts are not enabled, CLI clears the IF
#       flag in the EFLAGS register. No other flags are affected. Clearing the
#       IF flag causes the processor to ignore maskable external interrupts. The
#       IF flag and the CLI and STI instruction have no affect on the generation
#       of exceptions and NMI interrupts.
#       When protected-mode virtual interrupts are enabled, CPL is 3, and IOPL
#       is less than 3; CLI clears the VIF flag in the EFLAGS register, leaving
#       IF unaffected.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.GLOBAL cli
#-------------------------------------------------------------------------------
cli:
	cli
    ret
#*******************************************************************************

