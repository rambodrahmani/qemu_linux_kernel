################################################################################
# File: a_delay.s
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 07/07/2019.
################################################################################

#-------------------------------------------------------------------------------
.text

.extern c_delay
.global a_delay              # dichiarazione necessaria per la funzione ini()
a_delay:             # primitiva INT $240
	call c_delay
	iretq
