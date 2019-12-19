		.data
fmt:	.asciz	"r1 = %d\n"

		.text
		.globl main

main:		stmfd sp!, {lr}
		mov r3, #34
		mov r2, #37
		and r1, r2, r3
print:		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
