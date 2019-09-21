#*******************************************************************************
# File: es1.s
#       Contains the Assembly translation for es1.cpp.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 21/09/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _ZN2clC1Ec3st1                                  # cl::cl(char c, st1 s2)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
#  i            -17
#  s2           -13
#  c            -9
#  &this        -8
#  %rbp         0
#-------------------------------------------------------------------------------
_ZN2clC1Ec3st1:
# set stack locations labels:
    .set this, -8
    .set c,    -9
    .set s2,   -13
    .set i,    -17

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $24, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack:
    movq %rdi, this(%rbp)
    movb %sil, c(%rbp)
    movl %edx, s2(%rbp)

# for loop 1 initialization:
    movl $0, i(%rbp)                # i = 0

for:
    cmpl $8, i(%rbp)                # check if i < 8
    jge  finefor                    # end for loop (i >= 8)

# for loop 1 body:
    movq   this(%rbp), %rdi         # &this -> %rdi
    movslq i(%rbp), %rcx            # i -> %rcx
    movsbq c(%rbp), %rax            # c -> %rax
    addq   %rcx, %rax               # c + i -> %rax
    movb   %al, (%rdi, %rcx, 1)     # s.vc[i] = c + i

    incl i(%rbp)                    # i++
    jmp  for                        # loop again

finefor:

# for loop 2 initialization
    movl $0, i(%rbp)                # i = 0

for1:
    cmpl $4, i(%rbp)                # check if i < 4
    jge  finefor1                   # end loop (i >= 4)

# for loop 2 body:
    movslq i(%rbp), %rcx            # i -> %rcx
    movq   this(%rbp), %rdi         # &this -> %rdi
    leaq   s2(%rbp), %rsi           # &s2 -> %rsi
    movb   (%rsi, %rcx, 1), %al     # s2.vc[i] -> %al
    movb   (%rdi, %rcx, 1), %bl     # s.vc[i] -> %bl
    movsbq %al, %rax
    movsbq %bl, %rbx
    subq   %rbx, %rax               # s2.vc[i] - s.vc[i] -> %eax
    movq   %rax, 8(%rdi, %rcx, 8)   # v[i] = s2.vc[i] - s.vc[i];

    incl i(%rbp)                    # i++
    jmp  for1                       # loop again

finefor1:

    movq  this(%rbp), %rax          # return initialized object address
    leave                           # movq %rbp, %rsp; popq rbp
    ret

#-------------------------------------------------------------------------------
.GLOBAL _ZN2cl5elab1ER3st13st2                 # void cl::elab1(st1& s1, st2 s2)
#-------------------------------------------------------------------------------
# activation frame:
# -----------------
#  i            -76
#  cla.s        -72
#  cla.v[0]     -64
#  cla.v[1]     -56
#  cla.v[2]     -48
#  cla.v[3]     -40
#  s2 [MSB]     -32
#  s2 [LSB]     -24
#  &s1          -16
#  &this        -8
#  %rbp         0
#-------------------------------------------------------------------------------
_ZN2cl5elab1ER3st13st2:
# set stack locations labels:
    .set this, -8
    .set s1,   -16
    .set s2,   -32
    .set cla,  -72
    .set i,    -76

# prologue: activation frame
    pushq %rbp
    movq  %rsp, %rbp
    subq  $80, %rsp                 # reserve stack space for actual arguments

# copy actual arguments to the stack
    movq %rdi, this(%rbp)
    movq %rsi, s1(%rbp)
    movq %rdx, s2(%rbp)
    movq %rcx, s2+8(%rbp)

# cl cla('S', s1);
    leaq cla(%rbp), %rdi
    movb $'C', %sil
    movl (%rsi), %edx
    call _ZN2clC1Ec3st1

# for loop inizitialization
    movl $0, i(%rbp)                # i = 0

for2:
    mcpl $4, i(%rbp)                # check if i < 4
    jge  finefor2                   # end for loop (I >= 4)

# for loop body:


finefor2:

    leave                           # movq %rbp, %rsp; popq %rbp
    ret

#*******************************************************************************

