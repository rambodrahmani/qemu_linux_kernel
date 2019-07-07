#include "def.h"

.global load_gdt
load_gdt:
        lgdt    gdt_pointer             // carichiamo la nuova GDT

        movw $SEL_SYS_TSS, %cx
        ltr %cx
        
        retq
