#*******************************************************************************
# File: es1.s
#       Contains the Assembly translation for es1.cpp.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 14/09/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1EcR3st2                                # cl::cl(char c, st2& s2)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# i             -28
# &s2           -24
# c             -9
# &this         -8
# %rbp          0
#-------------------------------------------------------------------------------
_ZN2clC1EcR3st2:
# set stack locations labels:
    .set this, -8
    .set c,    -9
    .set s2,   -24
    .set i,    -28

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $32, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack:
    movq %rdi, this(%rbp)
    movb %sil, c(%rbp)
    movq %rdx, s2(%rbp)

# for loop initialization:
    movl $0, i(%rbp)                # i = 0

for:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor                    # end for loop (i <= 4)

# for loop body:
    movslq i(%rbp), %rcx            # i -> %rcx
    movq   this(%rbp), %rdi         # &this -> %rdi
    movb   c(%rbp), %al             # c -> %al
    movb   %al, (%rdi, %rcx, 1)     # c1.vc[i] = c;
    movb   %al, 4(%rdi, %rcx, 1)    # c2.vc[i] = c;
    incb   c(%rbp)                  # c++

    movq   s2(%rbp), %rsi           # &s2 -> %rsi
    movslq (%rsi, %rcx, 4), %rbx    # s2.vd[i] -> %rbx
    movsbq 4(%rdi, %rcx, 1), %rax   # c2.vc[i] -> %rax
    addq   %rax, %rbx
    movq   %rbx, 8(%rdi, %rcx, 8)

    incl i(%rbp)                    # i++
    jmp  for                        # loop again

finefor:

    leave                           # movq %rbp, %rsp; popq %rbp
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1E3st13st2                   # void cl::elab1(st1 s1, st2 s2)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# cla.c1/c2     -72
# cla.v[0]      -64
# cla.v[1]      -56
# cla.v[2]      -48
# cla.v[3]      -40
# s2 [MSB]      -32
# s2 [LSB]      -24
# i             -16
# s1            -12
# &this         -8
# %rbp          0
#-------------------------------------------------------------------------------
_ZN2cl5elab1E3st13st2:
# set stack locations labels:
    .set this, -8
    .set s1,   -12
    .set i,    -16
    .set s2,   -32
    .set cla,  -72

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $72, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack:
    movq %rdi, this(%rbp)
    movl %esi, s1(%rbp)
    movq %rdx, s2(%rbp)
    movq %rcx, s2+8(%rbp)

# cl cla('a', s2);
    leaq cla(%rbp), %rdi
    movb $'a', %sil
    leaq s2(%rbp), %rdx
    call _ZN2clC1EcR3st2

# for loop initialization
    movl $0, i(%rbp)                # i = 0

for1:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor1                   # end for loop (i >= 4)

# for loop body
    

finefor1:

    leave                           # movq %rbp, %rsp; popq %rbp
    ret

#*******************************************************************************

