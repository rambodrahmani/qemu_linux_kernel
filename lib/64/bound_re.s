################################################################################
# File: bound_re.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global bound_re
bound_re:
	movq $5, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

