		.data
fmt:	.asciz	"r1, r2, r3 = %d, %d, %d\n"

		.text
		.globl main

main:		stmfd sp!, {lr}
		mov r3, #77
		mov r2, #37
		add r1, r2, r3
		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
