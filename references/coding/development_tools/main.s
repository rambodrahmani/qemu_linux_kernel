#*******************************************************************************
# File: main.s
#       Output result for:
#           g++ -S main.cpp
#
#       The C++ (or C) compiler has one input file containing the source code
#       and produces one output file containing the Assembly version. It is a
#       quite complex tool and the time being our only goal is to have a general
#       idea of its internal processes.
#
# Author: Rambod Rahmani <rambodrahmani@autistici.org>
#         Created on 03/08/2019.
#*******************************************************************************

#-------------------------------------------------------------------------------
.DATA
.GLOBAL var1, var2

# The .quad directive is used to define 64 bit numeric value(s).
var1:
    .QUAD   8

var2:
    .QUAD   4

#-------------------------------------------------------------------------------
.TEXT
.GLOBAL _start
#-------------------------------------------------------------------------------
_start:
	pushq	%RBP
	movq	%RSP, %RBP
	movq	var2(%RIP), %RAX
	movq	%RAX, %RDI
	call	foo
	movq	%RAX, var1(%RIP)
	movq	var1(%RIP), %RAX
	popq	%RBP
	ret
#*******************************************************************************

