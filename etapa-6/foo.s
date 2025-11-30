	.file	"foo.c"
	.text
	.globl	a
	.data
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.long	3
	.globl	b
	.align 4
	.type	b, @object
	.size	b, 4
b:
	.long	4
	.globl	c
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.long	-1
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	a@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, -4(%rbp)
	movq	a@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	movl	-4(%rbp), %eax
	addl	%eax, %edx
	movq	c@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	addl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 14.3.0"
	.section	.note.GNU-stack,"",@progbits
