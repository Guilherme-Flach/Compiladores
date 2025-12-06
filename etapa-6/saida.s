  .data
  .globl g_soma
  .align 4
  .type g_soma, object
  .size g_soma, 4
g_soma:
.long 0
  .globl g_mult
  .align 4
  .type g_mult, object
  .size g_mult, 4
g_mult:
.long 0

.text
  .globl	main
  .type	main, @function
main:
pushq	%rbp
movq %rsp, %rbp
    subq $2048, %rsp
    movl $0, -1028(%rbp)

    movl -1028(%rbp), %eax
    movl %eax, g_soma(%rip)

    movl $2, -1032(%rbp)

    movl -1032(%rbp), %eax
    movl %eax, g_mult(%rip)

    movl $10, -1036(%rbp)

    movl -1036(%rbp), %eax
    movl %eax, -8(%rbp)

    movl $5, -1040(%rbp)

    movl $0, %eax
   subl -1040(%rbp), %eax
   movl %eax, -1044(%rbp)

    movl -1044(%rbp), %eax
    movl %eax, -4(%rbp)

    jmp .L4

.L5:
    movl -4(%rbp), %eax
    movl %eax, -1060(%rbp)

    movl $0, -1064(%rbp)

    movl -1060(%rbp), %eax
    cmpl -1064(%rbp), %eax
    setne %al
    movzbl %al, %eax
    movl %eax, -1068(%rbp)

    movl -1068(%rbp), %eax
    cmpl $0, %eax
    je .L2
    jmp .L1

.L1:
    movl g_soma(%rip), %eax
    movl %eax, -1072(%rbp)

    movl -4(%rbp), %eax
    movl %eax, -1076(%rbp)

    movl g_mult(%rip), %eax
    movl %eax, -1080(%rbp)

    movl -1076(%rbp), %eax
    imull -1080(%rbp), %eax
    movl %eax, -1084(%rbp)

    movl -1072(%rbp), %eax
    addl -1084(%rbp), %eax
    movl %eax, -1088(%rbp)

    movl -1088(%rbp), %eax
    movl %eax, g_soma(%rip)

    jmp .L3

.L2:
    movl -8(%rbp), %eax
    movl %eax, -1092(%rbp)

    movl $50, -1096(%rbp)

    movl -1092(%rbp), %eax
    addl -1096(%rbp), %eax
    movl %eax, -1100(%rbp)

    movl -1100(%rbp), %eax
    movl %eax, -8(%rbp)

.L3:
    movl -4(%rbp), %eax
    movl %eax, -1104(%rbp)

    movl $1, -1108(%rbp)

    movl -1104(%rbp), %eax
    addl -1108(%rbp), %eax
    movl %eax, -1112(%rbp)

    movl -1112(%rbp), %eax
    movl %eax, -4(%rbp)

.L4:
    movl -4(%rbp), %eax
    movl %eax, -1048(%rbp)

    movl $5, -1052(%rbp)

    movl -1048(%rbp), %eax
    cmpl -1052(%rbp), %eax
    setle %al
    movzbl %al, %eax
    movl %eax, -1056(%rbp)

    movl -1056(%rbp), %eax
    cmpl $0, %eax
    je .L6
    jmp .L5

.L6:
    movl g_soma(%rip), %eax
    movl %eax, -1116(%rbp)

    movl -1116(%rbp), %eax
    xorl $1, %eax
    movl %eax, -1120(%rbp)

    movl -1120(%rbp), %eax
    cmpl $0, %eax
    je .L8
    jmp .L7

.L7:
    movl -8(%rbp), %eax
    movl %eax, -1124(%rbp)

    movl -1124(%rbp), %eax

leave
ret
    jmp .L9

.L8:
    movl $0, -1128(%rbp)

    movl -1128(%rbp), %eax

leave
ret
.L9:
.L_END_FUNC:
.LFE0:
  .ident	"GCC: (GNU) 14.3.0"
  .section .note.GNU-stack,"",@progbits
