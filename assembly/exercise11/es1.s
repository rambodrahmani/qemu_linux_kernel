#*******************************************************************************
# File: es1.s
#       Contains the Assembly translation for es1.cpp.
#
#       Compile with:
#           g++ -o es1 -fno-elide-constructors es1.s prova1.cpp
#
#       Test your result with:
#           ./es1 | diff - es1.out
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 18/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1ER3st1                                        # cl::cl(st1 & ss)
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# i             -24
# &ss           -16
# this          -8
# old %rbp      0 <- %rbp
# %rip
#-------------------------------------------------------------------------------
_ZN2clC1ER3st1:
    pushq   %rbp            # prologue
    movq    %rsp, %rbp
    subq    $24, %rsp       # memory space for actual arguments

# copy actual arguments to the stack
    movq    %rdi, -8(%rbp)      # this
    movq    %rsi, -16(%rbp)     # &ss

# for loop, initialization
    movq    $0, -24(%rbp)       # i = 0

for1:
    cmpq    $4, -24(%rbp)       # i < 4
    jge     finefor1            # end loop (i >= 4)

# for loop body
    movq    -8(%rbp), %rdi          # this -> %rdi
    movq    -16(%rbp), %rsi         # &ss -> %rax
    movq    -24(%rbp), %rcx         # i -> %rcx

    movslq  (%rsi, %rcx, 4), %rax   # ss.v[i] -> %rax
    movb    %al, (%rdi, %rcx, 1)    # v1[i] = ss.vi[i];

    shr     $1, %rax                # v1[i] / 2 -> %rax
    movq    %rax, 8(%rdi, %rcx, 8)  # v2[i] = v1[i] / 2;

    movslq  (%rsi, %rcx, 4), %rax   # ss.v[i] -> %rax
    shl     $1, %al                 # 2 * v1[i] -> %rax
    movb    %al,  4(%rdi, %rcx, 1)  # v3[i] = 2 * v1[i];

    incq    -24(%rbp)       # i++
    jmp     for1            # loop again

finefor1:
    movq    -8(%rbp), %rax      # return initialized object address
    leave
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2clC1E3st1Pi                              # cl::cl(st1 s1, int ar2[])
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# i             -40
# &ar2          -32
# s2 (LSB)      -24
# s1 (MSB)      -16
# this          -8
# old %rbp      0 <- %rbp
# %rip
#-------------------------------------------------------------------------------
_ZN2clC1E3st1Pi:
    pushq   %rbp            # prologue
    movq    %rsp, %rbp
    subq    $40, %rsp       # memory space for actual arguments

# copy actual arguments to the stack
    movq    %rdi, -8(%rbp)      # this
    movq    %rsi, -24(%rbp)     # s1 (LSB)
    movq    %rdx, -16(%rbp)     # s1 (MSB)
    movq    %rcx, -32(%rbp)     # &ar2

# for loop, initialization
    movq    $0, -40(%rbp)       # i = 0

for2:
    cmpq    $4, -40(%rbp)       # i < 4
    jge     finefor2            # end loop (i >= 4)

# for loop body
    movq    -8(%rbp), %rdi              # this -> %rdi
    movq    -40(%rbp), %rcx             # i -> %rcx
    movslq  -24(%rbp, %rcx, 4), %rax    # s1.vi[i] -> %rax
    movb    %al, (%rdi, %rcx, 1)        # v1[i] = s1.vi[i];
    shr     $2, %rax                    # v1[i] / 4;
    movq    %rax, 8(%rdi, %rcx, 8)      # v2[i] = v1[i] / 4;
    movq    -32(%rbp), %rsi             # &ar2 -> %rsi
    movl    (%rsi, %rcx, 4), %eax       # ar2[i] -> %eax
    shl     $1, %eax                    # 2 * ar2[i] -> %eax
    movb    %al, 4(%rdi, %rcx, 1)       # v3[i] = 2 * ar2[i];

    incq    -40(%rbp)       # i++
    jmp     for2            # loop again

finefor2:
    movq    -8(%rbp), %rax      # return initialized object address
    leave
    ret
#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1EPc3st2                   # cl cl::elab1(char ar1[], st2 s2)
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# i             -96
# cla.v1/.v3    -88
# cla.v2        -80
# cla.v2        -72
# cla.v2        -64
# cla.v2        -56
# s1            -48
#               -40
# s2            -32
# &ar1          -28
# this          -16
# res. addr.    -8
# old %rbp      0 <- %rbp
# %rip
#-------------------------------------------------------------------------------
_ZN2cl5elab1EPc3st2:
    .set    i,    -96       # index labels
    .set    cla,  -88
    .set    s1,   -48
    .set    s2,   -32
    .set    ar1,  -24
    .set    this, -16
    .set    indo, -8

    pushq   %rbp                # prologue
    movq    %rsp, %rbp
    subq    $96,  %rsp          # memory space for actual arguments

    movq    %rdi, indo(%rbp)    # result address
    movq    %rsi, this(%rbp)    # this
    movq    %rdx, ar1(%rbp)     # &ar1
    movq    %rcx, s2(%rbp)      # s2

# for loop 1, initialization
    movq    $0, i(%rbp)         # i = 0

for3:
    cmpq    $4, i(%rbp)         # i < 4
    jge     finefor3            # end loop (i >= 4)

# for loop 1 body
    movq    this(%rbp), %rdi         # this -> %rdi
    movq    i(%rbp), %rcx            # i -> %rcx
    movq    ar1(%rbp), %rsi          # &ar1 -> %rsi
    movsbq  (%rsi, %rcx, 1), %rax    # ar1[i] -> %rax
    addq    %rcx, %rax               # ar1[i] + i -> %rax
    movl    %eax, s1(%rbp, %rcx, 4)  # s1.vi[i] = ar1[i] + i;

    incq    i(%rbp)     # i++
    jmp     for3        # loop again

finefor3:

    leaq    cla(%rbp), %rdi     # actual arguments for constructor
    leaq    s1(%rbp), %rsi
    call    _ZN2clC1ER3st1      # call constructor

# for loop 2, initialization
    movq    $0, i(%rbp)

for4:
    cmpq    $4, i(%rbp)         # i < 4
    jge     finefor4            # end loop (i >= 4)

# for loop 2 body
    movq    this(%rbp), %rdi           # this -> %rdi
    movq    i(%rbp), %rcx              # i -> %rcx
    movb    s2(%rbp, %rcx, 1), %al     # s2.vd[i] -> %rax
    movb    %al, cla+4(%rbp, %rcx, 1)  # cla.v3[i] = s2.vd[i];

    incq    i(%rbp)
    jmp     for4

finefor4:

    leaq     cla(%rbp), %rsi        # &cla -> %rsi
    movq     indo(%rbp), %rdi       # result address -> %rdi
    movabsq	 $5, %rcx               # rep count
    rep      movsq                  # move qword from %rsi to %rdi
    movq     indo(%rbp), %rax       # return initialized object address

    leave
    ret
#*******************************************************************************

