################################################################################
# File: commm_exc.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 06/07/2019.
################################################################################

#-------------------------------------------------------------------------------
#include "def.h"
.global comm_exc
comm_exc:
	call gestore_eccezioni
	hlt
