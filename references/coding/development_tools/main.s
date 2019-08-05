.DATA

.global var1, var2

# The .quad directive is used to define 64 bit numeric value(s).
var1:
    .QUAD   8

var2:
    .QUAD   4

.TEXT

.GLOBAL _start

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

