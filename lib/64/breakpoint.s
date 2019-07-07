################################################################################
# File: breakpoint.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global breakpoint
breakpoint:
	movq $3, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

