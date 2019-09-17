.text
.global _ZN2clC1EcR3st2

_ZN2clC1EcR3st2:

.set	i,-28
.set	s2,-24
.set	c,-9
.set	this,-8
	
	pushq	%rbp
	movq	%rsp,%rbp
	subq	$32,%rsp
	
	movq	%rdi,this(%rbp)
	movb	%sil,c(%rbp)
	movq	%rdx,s2(%rbp)
	
	movq	$0,-32(%rbp)

	_for1:
		cmpl	$4,i(%rbp)
		jge		_fine_for1
		
		movslq	i(%rbp),%rcx
	
		movb	c(%rbp),%al	
		addb	%cl,%al			#c + i
		
		movq	this(%rbp),%rsi
		movb	%al,(%rsi,%rcx,1)	#s.vc[i] = c + i;
		
		movq	s2(%rbp),%rdi
		movl	(%rdi,%rcx,4),%eax
		movsbl	(%rsi,%rcx,1),%ebx
		addl	%ebx,%eax			#s2.vd[i] + s.vc[i] in eax
		movslq	%eax,%rax
		movq	%rax,8(%rsi,%rcx,8)	#v[i] = s2.vd[i] + s.vc[i]
		
		incl	i(%rbp)
		jmp		_for1

	_fine_for1:
	movq	this(%rbp),%rax
	leave
	ret

.global _ZN2cl5elab1E3st13st2

_ZN2cl5elab1E3st13st2:

	.set	i2,			-76	
	.set	cla_s_vc,	-72
	.set	cla_v,		-64
	.set	s2_vd,		-32
	.set	s1_vc,		-12
	.set	this1,		-8

	pushq	%rbp
	movq	%rsp,%rbp
	subq	$80,%rsp
	
	movq	%rdi,this(%rbp)
	movl	%esi,s1_vc(%rbp)
	movq	%rdx,-24(%rbp)
	movq	%rcx,-32(%rbp)

	leaq	-72(%rbp),%rdi
	movb	$'a',%sil
	leaq	-32(%rbp),%rdx
	call	_ZN2clC1EcR3st2

	movl	$0,i2(%rbp)

	_for2:
		cmpl	$4,i2(%rbp)
		jge		_fine_for2
			
		movslq	i2(%rbp),%rcx

		movq	this1(%rbp),%rdi
		movb	(%rdi,%rcx,1),%al		#s.vc[i] in al
		movb	s1_vc(%rbp,%rcx,1),%bl	#s1.vc[i] in bl
		cmpb	%al,%bl
		jl		_salta_if

		/*CORPO IF*/
		incq	%r9
		movq	this1(%rbp),%rdi
		leaq	cla_s_vc(%rbp),%r8
		movb	(%r8,%rcx,1),%al
		movb	%al,(%rdi,%rcx,1)	#s.vc[i] = cla.s.vc[i];
		
		leaq	cla_v(%rbp),%r8
		movq	(%r8,%rcx,8),%rbx	#cla.v[i] in %rbx
		leaq	8(%rdi),%r8
		movq	%rbx,(%r8,%rcx,8)	#v[i] = cla.v[i];
		
		_salta_if:
		incl	i2(%rbp)
		jmp		_for2

	_fine_for2:

	movq %r9,%rax
	leave
	ret




