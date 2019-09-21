#*******************************************************************************
# File: es1.s
#       Contains the Assembly translation for es1.cpp.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 21/09/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1E3st1                                           # cl::cl(st1 ss)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# i             -16
# ss            -12
# &this         -8
# %rbp          0
#-------------------------------------------------------------------------------
_ZN2clC1E3st1:
# set stack locations labels:
    .set this, -8
    .set ss,   -12
    .set i,    -16

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $16, %rsp                 # reserver stack space for actual arguments

# copy actual arguments to the stack:
    movq %rdi, this(%rbp)
    movl %esi, ss(%rbp)

# for loop initialization:
    movl $0, i(%rbp)                # i = 0

for:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor                    # end for loop (i >= 4)

# for loop body:
    movslq i(%rbp), %rcx            # i -> %rcx
    leaq   ss(%rbp), %rsi           # &ss -> %rsi
    movq   this(%rbp), %rdi         # &this -> %rdi
    movsbq (%rsi, %rcx, 1), %rax    # ss.vi[i] -> %rax
    movb   %al, (%rdi, %rcx, 1)     # v1[i] = ss.vi[i]
    movb   %al, 4(%rdi, %rcx, 1)    # v2[i] = ss.vi[i]
    addq   %rax, %rax               # ss.vi[i] + ss.vi[i] -> %rax
    movq   %rax, 8(%rdi, %rcx, 8)   # v3[i] = ss.vi[i] + ss.vi[i];

    incl i(%rbp)                    # i++
    jmp  for                        # loop again

finefor:

    movq  this(%rbp), %rax          # return initialized object address
    leave                           # movq %rsp, %rbp; popq %rbp
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2clC1E3st1Pl                             # cl::cl(st1 s1, long ar2[])
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# &ar2          -24
# s1            -12
# &this         -8
# %rbp          0
#-------------------------------------------------------------------------------
_ZN2clC1E3st1Pl:
# set stack locations labels:
    .set this, -8
    .set s1,   -12
    .set ar2,  -24

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $24, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack:
    movq %rdi, this(%rbp)
    movl %esi, s1(%rbp)
    movq %rdx, ar2(%rbp)

# for loop initialization:
    movl $0, i(%rbp)                # i = 0

for1:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor1                   # end for loop (i >= 4)

# for loop body
    movslq i(%rbp), %rcx            # i -> %rcx
    movq   this(%rbp), %rdi         # &this -> %rdi
    leaq   s1(%rbp), %rsi           # &s1 -> %rsi
    movb   (%rsi, %rcx, 1), %al     # s1.vi[i] -> %al
    movb   %al, (%rdi, %rcx, 1)     # v1[i] = s1.vi[i]
    movb   %al, 4(%rdi, %rcx, 1)    # v2[i] = s1.vi[i]
    movq   ar2(%rbp), %rsi          # &ar2 -> %rsi
    movq   (%rsi, %rcx, 8), %rax    # ar2[i] -> %rax
    movq   %rax, 8(%rdi, %rcx, 8)   # v3[i] = ar2[i]

    incl i(%rbp)                    # i++
    jmp  for1                       # loop again

finefor1:

    movq  this(%rbp), %rax          # return initialized object address
    leave                           # movq %rsp, %rbp; popq %rbp
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1EPc3st2                   # cl cl::elab1(char ar1[], st2 s2)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# cla.v1/v2     -88
# cla.v3[3]     -80
# cla.v3[2]     -72
# cla.v3[1]     -64
# cla.v3[0]     -56
# i             -48
# s1            -44
# s2 [MSB]      -40
# s2 [LSB]      -32
# &ar1          -24
# &this         -16
# &return       -8
# %rbp          0
#-------------------------------------------------------------------------------
_ZN2cl5elab1EPc3st2:
# set stack location labels:
    .set return, -8
    .set this,   -16
    .set ar1,    -24
    .set s2,     -40
    .set s1,     -44
    .set i,      -48
    .set cla,    -88

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $88, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack
    movq %rdi, return(%rbp)
    movq %rsi, this(%rbp)
    movq %rdx, ar1(%rbp)
    movq %rcx, s2(%rbp)
    movq %r8,  s2+8(%rbp)

# for loop 1 initialization:
    movl $0, i(%rbp)                # i = 0

for2:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor2                   # end for loop (i >= 4)

# for loop 1 body:
    movslq i(%rbp), %rcx            # i -> %rcx
    leaq   s1(%rbp), %rdi           # &s1 -> %rdx
    movq   ar1(%rbp), %rsi          # &ar1 -> %rsi
    movb   (%rsi, %rcx, 1), %al     # ar1[i] -> %al
    movb   %al, (%rdi, %rcx, 1)     # s1.vi[i] = ar1[i];

    incl i(%rbp)                    # i++
    jmp  for2                       # loop again

finefor2:

# cl cla(s1);
    leaq cla(%rbp), %rdi
    movl s1(%rbp), %esi
    call _ZN2clC1E3st1

# for loop 2 initialization
    movl $0, i(%rbp)                # i = 0

for3:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor3                   # end for loop (i >= 4)

# for loop 2 body:
    movslq i(%rbp), %rcx            # i -> %rcx
    leaq   s2(%rbp), %rsi           # &s2 -> %rsi
    leaq   cla(%rbp), %rdi          # &cla -> %rdi
    movslq (%rsi, %rcx, 4), %rax    # s2.vd[i] -> %rax
    movq   %rax, 8(%rdi, %rcx, 8)   # cla.v3[i] = s2.vd[i]

    incl i(%rbp)                    # i++
    jmp  for3                       # loop again

finefor3:

# copy cla into the memory location addressed by &return
    leaq cla(%rbp), %rsi
    movq return(%rbp), %rdi
    movabsq $5, %rcx
    rep movsq

# return initialized object address
    movq return(%rbp), %rax

    leave                           # movq %rsp, %rbp; popq %rbp
    ret

#*******************************************************************************

