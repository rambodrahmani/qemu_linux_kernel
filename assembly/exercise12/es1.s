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
#         Created on 19/07/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1E3st1                                           # cl::cl(st1 ss)
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# i             -32
# ss (LSB)      -24 -----------> ss.vi[0]   -24
# ss (MSB)      -16              ss.vi[1]   -20
# this          -8               ss.vi[2]   -16
# old %rbp      0 <- %rbp        ss.vi[3]   -12
# %rip
#-------------------------------------------------------------------------------
_ZN2clC1E3st1:
    pushq   %rbp            # prologue
    movq    %rsp, %rbp
    subq    $32, %rsp       # memory space for actual arguments

# copy actual arguments to the stack
    movq    %rdi, -8(%rbp)      # this
    movq    %rsi, -24(%rbp)     # ss (LSB)
    movq    %rdx, -16(%rbp)     # ss (MSB)

# for loop, initialization
    movq    $0, -32(%rbp)       # i = 0

for1:
    cmpq    $4, -32(%rbp)       # i < 4
    jge     finefor1            # end loop (i >= 4)

# for loop body
    movq    -8(%rbp), %rdi            # this -> %rdi
    movq    -32(%rbp), %rcx           # i -> %rcx
    movslq  -24(%rbp, %rcx, 4), %rax  # ss.vi[i] -> %rax
    movb    %al, 32(%rdi, %rcx, 1)    # v1[i] = ss.vi[i];
    shl     $1, %rax                  # ss.vi[i] * 2 -> %rax
    movq    %rax, (%rdi, %rcx, 8)     # v2[i] = ss.vi[i] * 2;
    movb    %al, 36(%rdi, %rcx, 1)    # v3[i] = 2 * ss.vi[i];

    incq    -32(%rbp)           # i++
    jmp     for1                # loop again

finefor1:
    movq    -8(%rbp), %rax      # return initialized object address
    leave
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2clC1ER3st1Pi                           # cl::cl(st1 & s1, int ar2[])
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# this          -8
# old %rbp      0 <- %rbp
# %rip
#-------------------------------------------------------------------------------
_ZN2clC1ER3st1Pi:
    pushq   %rbp                # prologue
    movq    %rsp, %rbp
    subq    $1, %rsp            # memory space for actual arguments

# copy actual arguments to the stack
    

#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1EPc3st2                   # cl cl::elab1(char ar1[], st2 s2)
#-------------------------------------------------------------------------------
# activation record:
# ------------------
# this          -8
# old %rbp      0 <- %rbp
# %rip
#-------------------------------------------------------------------------------

#*******************************************************************************

