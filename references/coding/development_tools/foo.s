.DATA
foovar1:
    .QUAD   5

foovar2:
    .QUAD   6

.TEXT

.global foo

foo:
    PUSHQ   %RBP
    MOVQ    %RSP, %RBP
    MOVQ    %RDI, %RAX
    MOVQ    foovar1, %RAX
    ADDQ    foovar2(%RIP), %RAX
    LEAVE   # [0]
    RET

# [0]
# l'istruzione leave equivale a fare :
#    mov %rbp, %rsp
#    popq %rbp

