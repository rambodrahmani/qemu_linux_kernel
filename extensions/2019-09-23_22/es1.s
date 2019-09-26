#*******************************************************************************
# File: es1.s
#       Contains the assembly translation for es1.cpp.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 26/09/2019
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1EcR3st2
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

# prologue: acitvation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $32, %rsp              # reserver stack space for actual arguments

# copy actual arguments to the stack
    movq %rdi, this(%rbp)
    movb %sil, c(%rbp)
    movq %rdx, s2(%rbp)

# for loop initialization
    movl $0, i(%rbp)                # i = 0

for:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor                    # end for loop (i >= 4)

# for loop body:
    movslq i(%rbp), %rcx            # i -> %rcx
    movq   this(%rbp), %rdi         # &this -> %rdi
    movb   c(%rbp), %al             # c-> %al
    movb   %al, 32(%rdi, %rcx, 1)   # c1.vc[i] = c
    movb   %al, 36(%rdi, %rcx, 1)   # c2.vc[i] = c
    incb   c(%rbp)                  # c++
    movq   s2(%rbp), %rsi           # &s2 -> %rsi
    movsbq (%rsi, %rcx, 1), %rax    # s2.vd[i] -> %rax
    movsbq 36(%rdi, %rcx, 1), %rbx  # c2.vc[i] -> %rbx
    addq   %rax, %rbx               # s2.vd[i] + c2.vc[i] -> %rbx
    movq   %rbx, (%rdi, %rcx, 8)    # v[i] = s2.vd[i] + c2.vc[i];

    incl i(%rbp)                    # i++
    jmp  for                        # loop again

finefor:

    movq  this(%rbp), %rax          # return initialized object address
    leave
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1E3st13st2
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
# i             -60
# cla.v[0]      -56
# cla.v[1]      -48
# cla.v[2]      -40
# cla.v[3]      -32
# cla.c1/c2     -24
# s2            -16
# s1            -12
# &this         -8
# %rbo          0
#-------------------------------------------------------------------------------
_ZN2cl5elab1E3st13st2:
# set stack locations labels:
    .set this, -8
    .set s1,   -12
    .set s2,   -16
    .set cla,  -56
    .set i,    -60

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $64, %rsp               # reserve stack space for actual arguments

# move actual arguments to the stack
    movq %rdi, this(%rbp)
    movl %esi, s1(%rbp)
    movl %edx, s2(%rbp)

# cl cla(’a’, s2);
    leaq cla(%rbp), %rdi
    movb $'a', %sil
    leaq s2(%rbp), %rdx
    call _ZN2clC1EcR3st2

# for loop initialization
    movl $0, i(%rbp)                # i = 0

for1:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor1                   # end for loop (i >= 4)

# for loop body:
    movslq i(%rbp), %rcx            # i -> %rcx
    movq   this(%rbp), %rdi         # &this -> %rdi
    leaq   s1(%rbp), %rsi           # &s1 -> %rsi

# if (c2.vc[i] <= s1.vc[i])
    movb   36(%rdi, %rcx, 1), %al   # c2.vc[i] -> %al
    movb   (%rsi, %rcx, 1), %bl     # s1.vc[i] -> %bl
    cmpb   %bl, %al                 # compare c2.vc[i] and s1.vc[i]
    jg     fineif                   # exit if (c2.vc[i] > s1.vc[i])
    leaq   cla(%rbp), %rsi          # &cla -> %rsi
    movsbq 36(%rsi, %rcx, 1), %rax  # cla.c2.vc[i] -> %rax
    addq   %rcx, %rax               # i + cla.c2.vc[i] -> %rax
    movb   %al, 32(%rdi, %rcx, 1)   # c1.vc[i] = i + cla.c2.vc[i];
    movq   (%rsi, %rcx, 8), %rax    # cla.v[i] -> %rax
    movq   %rcx, %rbx               # i -> %rbx
    subq   %rax, %rbx               # i - cla.v[i] -> %rbx
    movq   %rbx, (%rdi, %rcx, 8)    # v[i] = i - cla.v[i];

fineif:

    incl i(%rbp)                    # i++
    jmp  for1                       # loop again

finefor1:

    leave
    ret
#*******************************************************************************

