################################################################################
# File: coproc_so.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global coproc_so
coproc_so:
	movq $9, %rdi
	movq $0, %rsi
	popq %rdx
	jmp comm_exc

