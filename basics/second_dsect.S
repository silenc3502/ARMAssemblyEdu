		.data
i:		.word	0
j:		.word	1
fmt:	.asciz	"Hello, %d\n"
fmt2:	.asciz	"Hello, %s\n"
ch:		.byte	'A', 'B', 0
ary:	.word	0, 1, 2, 3, 4

		.text
		.globl main

main:	stmfd sp!, {lr}
		ldr r1, =i
		ldr r1, [r1]
		ldr r0, =fmt
		bl printf
		ldr r1, =j
		ldr r1, [r1]
		ldr r0, =fmt
		bl printf
		ldr r1, =ch
		ldr r0, =fmt2
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
