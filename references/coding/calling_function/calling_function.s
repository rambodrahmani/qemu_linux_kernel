#*******************************************************************************
# File: calling_function.s
#       This Assembly example shows how to call a function from the C++ STD
#       library.
#
#       Compile using:
#           g++ -no-pie calling_function.s -o calling_function
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 03/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.DATA
x:
    .QUAD   0
y:
    .QUAD   10
str:
    .STRING "value: %d\n"
#-------------------------------------------------------------------------------
.TEXT
.GLOBAL main
#-------------------------------------------------------------------------------
main:
    MOVQ  $str, %rdi   # first argument in %rdi: pointer to string
    MOVQ  y,    %rsi   # second argument in %rsi: value for %d
    MOVQ  $0,   %rax   # there are zero floating point args

    PUSHQ  %r10        # save the caller-saved registers
    PUSHQ  %r11        

    CALL  printf       # invoke printf

    POPQ  %r11         # restore the caller-saved registers
    POPQ  %r10         

    MOVQ  %rax, x      # save the result in x

    RET                # return from main function
#*******************************************************************************

