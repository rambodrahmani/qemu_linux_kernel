################################################################################
# File: invalid_opcode.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global invalid_opcode
invalid_opcode:
	movq $6, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

