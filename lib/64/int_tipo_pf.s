################################################################################
# File: int_tipo_pf.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global int_tipo_pf
int_tipo_pf:
	movq $14, %rdi
	popq %rsi
	popq %rdx
	jmp comm_exc

