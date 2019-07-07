#include "def.h"
.data
.balign 16
.global sys_tss
sys_tss:
        .space 236, 0
